"""
flask_app.py  –  Blackjack REST backend
----------------------------------------
Wraps your compiled C++ pybind11 module (blackjack_game) with a
stateful Flask session so the browser frontend can drive the game.

HOW TO COMPILE THE C++ MODULE (run once from project root):
  Windows (MSVC / MinGW not supported for pybind – use MSVC):
    python setup.py build_ext --inplace

  Or manually with cl.exe:
    cl /O2 /LD /MD /std:c++17
       src/Card.cpp src/Deck.cpp src/Player.cpp src/Dealer.cpp
       src/Game.cpp src/blackjack_bindings.cpp
       /I src
       /I "C:/Users/<YOU>/AppData/Local/Programs/Python/Python313/include"
       /I "C:/Users/<YOU>/AppData/Local/Programs/Python/Python313/Lib/site-packages/pybind11/include"
       /link /OUT:blackjack_game.pyd
       "C:/Users/<YOU>/AppData/Local/Programs/Python/Python313/libs/python313.lib"

  Linux / Mac:
    c++ -O2 -shared -fPIC -std=c++17
        src/*.cpp
        $(python3 -m pybind11 --includes)
        -o blackjack_game$(python3-config --extension-suffix)

HOW TO RUN:
    pip install flask flask-cors
    python flask_app.py
    Open blackjack.html in your browser (or visit http://localhost:5000)
"""

from flask import Flask, jsonify, request, session, send_from_directory
from flask_cors import CORS
import os, sys

# ── Try to import the compiled C++ module ──────────────────────────────────
try:
    import blackjack_game as bj
    CPP_AVAILABLE = True
except ImportError:
    CPP_AVAILABLE = False
    print("[WARNING] blackjack_game module not found – compile the C++ pybind11 "
          "module first.  See the docstring at the top of this file.")
    sys.exit(1)

# ── Flask setup ────────────────────────────────────────────────────────────
app = Flask(__name__, static_folder=".")
app.secret_key = os.urandom(24)   # session encryption key (ephemeral)
CORS(app, supports_credentials=True)

# ── Per-session game instance (stored via session cookie key → Python dict)
# Flask sessions are cookie-backed; we keep the heavyweight C++ object in a
# module-level dict keyed by session ID so it doesn't need to be serialised.
_GAMES: dict[str, bj.Game] = {}

def _get_game() -> bj.Game:
    sid = session.get("sid")
    if sid is None or sid not in _GAMES:
        sid = os.urandom(16).hex()
        session["sid"] = sid
        _GAMES[sid] = bj.Game()
    return _GAMES[sid]

def _reset_game() -> bj.Game:
    sid = session.get("sid")
    if sid and sid in _GAMES:
        del _GAMES[sid]
    return _get_game()

# ── Serialisation helpers ──────────────────────────────────────────────────

def _card(c: bj.Card) -> dict:
    return {"rank": c.getRankString(), "suit": c.getSuitString()}

def _hand(cards) -> list:
    return [_card(c) for c in cards]

def _game_state(g: bj.Game, hide_hole: bool = False) -> dict:
    """Return a JSON-serialisable snapshot of the full game state."""
    p = g.p
    d = g.d

    dealer_cards = _hand(d.Dealerhand)
    if hide_hole and len(dealer_cards) >= 2:
        dealer_cards[1] = {"rank": "?", "suit": "?"}   # hide hole card

    return {
        # dealer
        "dealerHand":       dealer_cards,
        "dealerValue":      d.Dealerhandvalue() if not hide_hole else None,
        "dealerBJ":         bool(d.dBJstatus),
        "dealerBust":       bool(d.Dealerbust()) if not hide_hole else False,

        # player main hand
        "playerHand":       _hand(p.hand),
        "playerValue":      p.handvalue(0),
        "playerBust":       bool(p.busted),
        "playerBJ":         bool(p.pBJstatus),
        "playerStanding":   bool(p.isStanding),
        "hasSurrendered":   bool(p.hasSurrendered),
        "insured":          bool(p.Insured),
        "didDoubleDown":    bool(p.diddoubledown),

        # split hand
        "didSplit":         bool(p.didsplit),
        "splitHand":        _hand(p.Splithand),
        "splitValue":       p.handvalue(1) if p.didsplit else 0,
        "splitBust":        bool(p.splithandbust),
        "splitStanding":    bool(p.isStandingf(1)),
        "splitBet":         int(p.Splitbet),

        # money
        "bankBalance":      int(p.Bankbalance),
        "betAmount":        int(p.betamt),

        # dealer upcard for insurance check
        "dealerUpcardIsAce": len(d.Dealerhand) > 0 and d.Dealerhand[0].isAce(),

        # available actions (computed here so frontend doesn't need logic)
        "canDoubleDown":    bool(p.candoubledown(0)),
        "canSplit":         bool(p.cansplit()),
        "canInsure":        (len(d.Dealerhand) > 0 and d.Dealerhand[0].isAce()
                             and not p.Insured and not p.didsplit),
        "canSurrender":     (len(p.hand) == 2 and not p.didsplit),
        "canSplitDoubleDown": bool(p.candoubledown(1)) if p.didsplit else False,
    }


# ══════════════════════════════════════════════════════════════════════════
#  ROUTES
# ══════════════════════════════════════════════════════════════════════════

@app.route("/")
def index():
    return send_from_directory(".", "blackjack.html")

# ── /api/state  (GET) ──────────────────────────────────────────────────────
@app.route("/api/state", methods=["GET"])
def get_state():
    g = _get_game()
    return jsonify({"ok": True, "state": _game_state(g)})

# ── /api/new_round  (POST) ────────────────────────────────────────────────
@app.route("/api/new_round", methods=["POST"])
def new_round():
    """Reset player/dealer hands but keep the bank balance."""
    g = _get_game()
    # Save balance BEFORE clearing (Clearhand resets betamt but not Bankbalance)
    balance = int(g.p.Bankbalance)
    # Manually clear hands instead of closeRound() to avoid stdout noise
    g.p.Clearhand()
    g.d.clearhand()
    # Restore balance (it's untouched by Clearhand but be explicit)
    g.p.Bankbalance = balance
    return jsonify({"ok": True, "state": _game_state(g)})

# ── /api/bet  (POST)  body: { amount: int } ───────────────────────────────
@app.route("/api/bet", methods=["POST"])
def place_bet():
    g = _get_game()
    data = request.get_json(force=True)
    amt = int(data.get("amount", 0))
    if amt <= 0:
        return jsonify({"ok": False, "error": "Invalid amount"}), 400
    if not g.p.hasbetamt(amt):
        return jsonify({"ok": False, "error": "Insufficient balance"}), 400
    g.p.Placebet(amt)
    return jsonify({"ok": True, "state": _game_state(g)})

# ── /api/clear_bet  (POST) ────────────────────────────────────────────────
@app.route("/api/clear_bet", methods=["POST"])
def clear_bet():
    g = _get_game()
    p = g.p
    # Return chips to balance
    p.Bankbalance += p.betamt
    p.betamt = 0
    return jsonify({"ok": True, "state": _game_state(g)})

# ── /api/deal  (POST) ─────────────────────────────────────────────────────
@app.route("/api/deal", methods=["POST"])
def deal():
    """Deal initial hands.  Returns state with hole card hidden."""
    g = _get_game()
    if g.p.betamt == 0:
        return jsonify({"ok": False, "error": "No bet placed"}), 400

    # Safety: force-clear any leftover cards from a previous round
    # (guards against new_round not being called or partial state)
    if len(g.p.hand) > 0 or len(g.d.Dealerhand) > 0:
        bet = int(g.p.betamt)
        g.p.Clearhand()
        g.d.clearhand()
        g.p.Bankbalance -= bet   # re-deduct since Clearhand returned it
        g.p.betamt = bet         # restore bet

    g.Initialhands()   # deals 2 cards to player + dealer

    # Check BJ statuses
    g.p.pBJstatus = g.p.hasBlackjackf()
    g.d.dBJstatus = g.d.DealerBlackjack()

    # If either has BJ go straight to result phase
    if g.p.pBJstatus or g.d.dBJstatus:
        g.Result()
        state = _game_state(g, hide_hole=False)
        state["phase"] = "result"
        return jsonify({"ok": True, "state": state})

    state = _game_state(g, hide_hole=True)
    state["phase"] = "player"
    return jsonify({"ok": True, "state": state})

# ── /api/hit  (POST)  body: { hand: 0|1 } ────────────────────────────────
@app.route("/api/hit", methods=["POST"])
def hit():
    g = _get_game()
    data = request.get_json(force=True)
    hp = int(data.get("hand", 0))
    g.p.hit(g.deck, hp)

    busted = g.p.bust(hp)
    if busted:
        if hp == 0: g.p.busted = True
        else:       g.p.splithandbust = True

    state = _game_state(g, hide_hole=True)
    state["phase"] = "player"
    state["justBusted"] = busted
    return jsonify({"ok": True, "state": state})

# ── /api/stand  (POST)  body: { hand: 0|1 } ──────────────────────────────
@app.route("/api/stand", methods=["POST"])
def stand():
    g = _get_game()
    data = request.get_json(force=True)
    hp = int(data.get("hand", 0))
    g.p.stand(hp)
    state = _game_state(g, hide_hole=True)
    state["phase"] = "player"
    return jsonify({"ok": True, "state": state})

# ── /api/double_down  (POST)  body: { hand: 0|1 } ────────────────────────
@app.route("/api/double_down", methods=["POST"])
def double_down():
    g = _get_game()
    data = request.get_json(force=True)
    hp = int(data.get("hand", 0))
    if not g.p.candoubledown(hp):
        return jsonify({"ok": False, "error": "Cannot double down"}), 400
    g.p.doubledown(g.deck, hp)
    if hp == 0: g.p.busted = g.p.bust(0)
    else:       g.p.splithandbust = g.p.bust(1)
    state = _game_state(g, hide_hole=True)
    state["phase"] = "player"
    return jsonify({"ok": True, "state": state})

# ── /api/split  (POST) ────────────────────────────────────────────────────
@app.route("/api/split", methods=["POST"])
def split():
    g = _get_game()
    if not g.p.cansplit():
        return jsonify({"ok": False, "error": "Cannot split"}), 400
    g.p.split(g.deck)
    state = _game_state(g, hide_hole=True)
    state["phase"] = "player"
    return jsonify({"ok": True, "state": state})

# ── /api/insurance  (POST) ────────────────────────────────────────────────
@app.route("/api/insurance", methods=["POST"])
def insurance():
    g = _get_game()
    insure_amt = g.p.betamt // 2
    if insure_amt > g.p.Bankbalance:
        return jsonify({"ok": False, "error": "Insufficient balance for insurance"}), 400
    g.p.Insured = True
    g.p.Bankbalance -= insure_amt
    state = _game_state(g, hide_hole=True)
    state["phase"] = "player"
    return jsonify({"ok": True, "state": state})

# ── /api/surrender  (POST) ────────────────────────────────────────────────
@app.route("/api/surrender", methods=["POST"])
def surrender():
    g = _get_game()
    if len(g.p.hand) != 2 or g.p.didsplit:
        return jsonify({"ok": False, "error": "Cannot surrender"}), 400
    g.p.hasSurrendered = True
    g.Result()
    state = _game_state(g, hide_hole=False)
    state["phase"] = "result"
    return jsonify({"ok": True, "state": state})

# ── /api/dealer_play  (POST) ──────────────────────────────────────────────
@app.route("/api/dealer_play", methods=["POST"])
def dealer_play():
    """
    Called by the frontend when all player hands are done.
    Runs DealerChance() then Result() and returns final state.
    """
    g = _get_game()
    g.DealerChance()
    g.Result()
    state = _game_state(g, hide_hole=False)
    state["phase"] = "result"
    return jsonify({"ok": True, "state": state})


# ══════════════════════════════════════════════════════════════════════════
if __name__ == "__main__":
    print("=" * 55)
    print("  Blackjack Flask Backend")
    print("  Open http://localhost:5000 in your browser")
    print("=" * 55)
    app.run(debug=True, port=5000)
