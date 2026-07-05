import os
import random

def generate_instance(filename, n, W, V):
    with open(filename, 'w') as f:
        # First line: W V
        f.write(f"{W} {V}\n")
        # Next n lines: weight volume value
        for _ in range(n):
            # Generate random but reasonable values
            # Using items that are somewhat proportional to the capacity to make it hard
            # or completely random
            weight = random.randint(1, max(1, W // (n // 4 + 1)))
            volume = random.randint(1, max(1, V // (n // 4 + 1)))
            value = random.randint(10, 100)
            f.write(f"{weight}\t{volume}\t{value}\n")

def main():
    data_dir = "data/experiments"
    os.makedirs(data_dir, exist_ok=True)
    
    # Grid of parameters (10 * 10 * 10 = 1000 combinações)
    N_values = [10, 12, 14, 16, 18, 20, 22, 24, 26, 28]
    W_values = [50, 100, 150, 200, 250, 300, 350, 400, 450, 500]
    V_values = [50, 100, 150, 200, 250, 300, 350, 400, 450, 500]
    
    # 10 instances per combination
    num_instances = 10
    
    for n in N_values:
        for w in W_values:
            for v in V_values:
                for i in range(num_instances):
                    filename = os.path.join(data_dir, f"inst_N{n}_W{w}_V{v}_{i}.txt")
                    generate_instance(filename, n, w, v)
                    
    print(f"Generated {len(N_values) * len(W_values) * len(V_values) * num_instances} instances in '{data_dir}'.")

if __name__ == '__main__':
    main()
