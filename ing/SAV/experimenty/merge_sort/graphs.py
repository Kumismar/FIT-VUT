import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

FIGSIZE = (8, 4)

sns.set_theme(style="whitegrid")

experiments = [
    ("results_correct.csv", "Korektní implementace", "correct"),
    ("results_cond.csv", "Chyba v řadící podmínce", "cond"),
    ("results_missing_copy.csv", "Chyba v závěrečném slučování polí", "array")
]


def plot_single_experiment(csv_file, title_base, output_prefix):
    if not os.path.exists(csv_file):
        return

    df = pd.read_csv(csv_file)

    # Calculate solve time, correct invalid solve times
    df["SolveTime"] = df["TotalTime"] - df["GenTime"]

    # Remove incorrect values (the solve times are approximate)
    df.loc[df["SolveTime"] < 0, "SolveTime"] = 0

    # Plot verification time
    plt.figure(figsize=FIGSIZE)
    sns.lineplot(data=df, x="Size", y="TotalTime", marker="o")
    plt.title(f"{title_base}: Čas verifikace")

    plt.xlabel("Velikost pole (N)")
    plt.ylabel("Čas (s)")
    plt.yscale("log")

    plt.tight_layout()
    plt.savefig(f"graph_{output_prefix}_total_time.png", dpi=400)
    plt.close()

    # Plot CNF formula size
    plt.figure(figsize=FIGSIZE)
    sns.lineplot(data=df, x="Size", y="Clauses", marker="s", color="orange")
    plt.title(f"{title_base}: Velikost CNF formule")

    plt.xlabel("Velikost pole (N)")
    plt.ylabel("Počet klauzulí")
    plt.yscale("log")

    plt.tight_layout()
    plt.savefig(f"graph_{output_prefix}_clauses.png", dpi=400)
    plt.close()

    # Plot generation vs solving time
    plt.figure(figsize=FIGSIZE)
    sns.lineplot(data=df, x="Size", y="GenTime",
                 marker="o", label="Generování formule")
    sns.lineplot(data=df, x="Size", y="SolveTime",
                 marker="o", label="Řešení formule SAT solverem")

    plt.title(f"{title_base}: Porovnání časů fází verifikace")
    plt.xlabel("Velikost pole (N)")
    plt.ylabel("Čas (s)")
    plt.yscale("log")

    plt.tight_layout()
    plt.savefig(f"graph_{output_prefix}_breakdown.png", dpi=400)
    plt.close()


for csv, title, prefix in experiments:
    plot_single_experiment(csv, title, prefix)

print("Graphs generated.")
