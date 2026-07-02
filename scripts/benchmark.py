import subprocess
import os
import csv
import re
import time

def run_method(method, input_file):
    cmd = ["./bin/programa.exe", method, input_file]
    try:
        start = time.time()
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=2.0)
        output = result.stdout
        
        profit = re.search(r"Maximum profit: ([\d.]+)", output)
        exec_time = re.search(r"Execution time: ([\d.]+)", output)
        nodes = re.search(r"Nodes visited: (\d+)", output)
        
        return {
            "method": method,
            "profit": profit.group(1) if profit else "0",
            "time": exec_time.group(1) if exec_time else "0",
            "nodes": nodes.group(1) if nodes else "N/A"
        }
    except subprocess.TimeoutExpired:
        return {"method": method, "profit": "TIMEOUT", "time": "2.0", "nodes": "TIMEOUT"}
    except Exception as e:
        print(f"Error running {method} on {input_file}: {e}")
        return None

def main():
    methods = ["dynamicProg", "backtracking", "branchBound"]
    data_dir = "data/experiments"
    results_file = "results_experiments.csv"
    
    instances = sorted([f for f in os.listdir(data_dir) if f.startswith("inst_") and f.endswith(".txt")])
    
    print(f"Found {len(instances)} instances.")
    
    with open(results_file, 'w', newline='') as csvfile:
        fieldnames = ['instance', 'n', 'W', 'V', 'method', 'profit', 'time', 'nodes']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        
        for idx, inst in enumerate(instances):
            if idx % 10 == 0:
                print(f"Processing instance {idx}/{len(instances)}...")
                
            m = re.search(r'inst_N(\d+)_W(\d+)_V(\d+)_', inst)
            n, W, V = 0, 0, 0
            if m:
                n, W, V = int(m.group(1)), int(m.group(2)), int(m.group(3))
            
            for method in methods:
                # The binary expects path relative to data dir or working dir?
                # Actually, main.cpp appends "data/" to the input:
                # input_file = "data/" + input_file;
                # So we should pass "experiments/inst..."
                res = run_method(method, "experiments/" + inst)
                if res:
                    writer.writerow({
                        "instance": inst,
                        "n": n,
                        "W": W,
                        "V": V,
                        "method": res["method"],
                        "profit": res["profit"],
                        "time": res["time"],
                        "nodes": res["nodes"]
                    })
    print(f"Done! Results saved to {results_file}")

if __name__ == "__main__":
    main()
