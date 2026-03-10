import gymnasium as gym
import numpy as np
import pickle
from collections import defaultdict

# Hyperparameters
EPISODES     = 500_000
ALPHA        = 0.1      # learning rate
GAMMA        = 0.99     # discount factor
EPSILON      = 1.0      # starting exploration rate
EPSILON_MIN  = 0.05
EPSILON_DECAY= 0.999995

env = gym.make("Blackjack-v1", sab=True)  # sab=True → standard Blackjack rules

# Q-table: state → [Q(stick), Q(hit)]
Q = defaultdict(lambda: np.zeros(env.action_space.n))

def choose_action(state, epsilon):
    if np.random.random() < epsilon:
        return env.action_space.sample()          # explore
    return int(np.argmax(Q[state]))               # exploit

def train():
    epsilon = EPSILON
    win_history = []

    for episode in range(1, EPISODES + 1):
        state, _ = env.reset()
        done = False
        final_reward = 0  # <-- track explicitly

        while not done:
            action = choose_action(state, epsilon)
            next_state, reward, terminated, truncated, _ = env.step(action)
            done = terminated or truncated

            best_next = np.max(Q[next_state])
            Q[state][action] += ALPHA * (reward + GAMMA * best_next - Q[state][action])
            state = next_state
            final_reward = reward  # <-- update each step, last value is terminal

        epsilon = max(EPSILON_MIN, epsilon * EPSILON_DECAY)
        win_history.append(final_reward)  # <-- use this instead of reward

        if episode % 100_000 == 0:
            win_rate = sum(1 for r in win_history[-10_000:] if r > 0) / min(len(win_history), 10_000)
            print(f"Episode {episode:>7} | Win rate (last 10k): {win_rate:.2%} | eps: {epsilon:.4f}")

    return Q

def evaluate(q_table, episodes=10_000):
    wins, draws, losses = 0, 0, 0
    for _ in range(episodes):
        state, _ = env.reset()
        done = False
        while not done:
            action = int(np.argmax(q_table[state]))
            state, reward, terminated, truncated, _ = env.step(action)
            done = terminated or truncated
        if reward > 0:   wins   += 1
        elif reward == 0: draws  += 1
        else:             losses += 1
    print(f"\nEvaluation over {episodes} games:")
    print(f"  Wins:   {wins}  ({wins/episodes:.2%})")
    print(f"  Draws:  {draws} ({draws/episodes:.2%})")
    print(f"  Losses: {losses} ({losses/episodes:.2%})")

if __name__ == "__main__":
    print("Training Q-learning agent on Blackjack...\n")
    Q = train()

    print("\nSaving Q-table...")
    with open("blackjack_qtable.pkl", "wb") as f:
        pickle.dump(dict(Q), f)

    evaluate(Q)
