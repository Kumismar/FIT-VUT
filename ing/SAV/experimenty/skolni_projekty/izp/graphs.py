import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Nastavení velikosti grafů
FIGSIZE = (8, 4)

sns.set_theme(style="whitegrid")

experiments = [
    ("results_set_union.csv", "Sjednocení množin", "set_union"),
    ("results_set_intersect.csv", "Průnik množin", "set_intersect"),
    ("results_closure_sym.csv", "Symetrický uzávěr", "closure_sym"),
    ("results_rel_domain.csv", "Doména relace", "rel_domain")
]


def plot_single_experiment(csv_file, title_base, output_prefix):
    if not os.path.exists(csv_file):
        print(f"File {csv_file} not found, skipping.")
        return

    df = pd.read_csv(csv_file)

    df["SolveTime"] = df["TotalTime"] - df["GenTime"]
    df.loc[df["SolveTime"] < 0, "SolveTime"] = 0

    # === GRAPH 1: Verification time ===
    plt.figure(figsize=FIGSIZE)

    sns.lineplot(data=df, x="Size", y="TotalTime",
                 marker="o", label="Celkový čas")

    timeouts = df[df["Result"] == "TIMEOUT"]
    if not timeouts.empty:
        plt.scatter(timeouts["Size"], timeouts["TotalTime"],
                    color="red", zorder=5, label="Timeout")

    plt.title(f"{title_base}: Čas verifikace")
    plt.xlabel("Velikost množiny (N)")
    plt.ylabel("Čas (s)")
    plt.yscale("log")
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"graph_{output_prefix}_total_time.png", dpi=400)
    plt.close()

    # === GRAF 2: CNF formula size ===
    df_clauses = df[df["Clauses"] > 0]

    if not df_clauses.empty:
        plt.figure(figsize=FIGSIZE)
        sns.lineplot(data=df_clauses, x="Size", y="Clauses",
                     marker="s", color="orange")
        plt.title(f"{title_base}: Velikost CNF formule")
        plt.xlabel("Velikost množiny (N)")
        plt.ylabel("Počet klauzulí")
        plt.yscale("log")
        plt.tight_layout()
        plt.savefig(f"graph_{output_prefix}_clauses.png", dpi=400)
        plt.close()

    # === GRAPH 3: Formula generation vs verification time ===
    df_finished = df[df["Result"] != "TIMEOUT"]

    if not df_finished.empty:
        plt.figure(figsize=FIGSIZE)
        sns.lineplot(data=df_finished, x="Size", y="GenTime",
                     marker="o", label="Generování formule")
        sns.lineplot(data=df_finished, x="Size", y="SolveTime",
                     marker="o", label="Řešení formule")

        plt.title(f"{title_base}: Rozpad času verifikace")
        plt.xlabel("Velikost množiny (N)")
        plt.ylabel("Čas (s)")
        plt.yscale("log")
        plt.legend()
        plt.tight_layout()
        plt.savefig(f"graph_{output_prefix}_breakdown.png", dpi=400)
        plt.close()


def plot_comparison(experiment_list):
    plt.figure(figsize=FIGSIZE)

    valid_data = False
    for csv_file, label, _ in experiment_list:
        if os.path.exists(csv_file):
            df = pd.read_csv(csv_file)
            sns.lineplot(data=df, x="Size", y="TotalTime",
                         marker="o", label=label)
            valid_data = True

    if valid_data:
        plt.title("Srovnání výpočetní náročnosti jednotlivých operací")
        plt.xlabel("Velikost množiny (N)")
        plt.ylabel("Celkový čas (s)")
        plt.yscale("log")
        plt.legend()
        plt.tight_layout()
        plt.savefig("graph_setcal_comparison.png", dpi=400)
        plt.close()


for csv, title, prefix in experiments:
    plot_single_experiment(csv, title, prefix)

plot_comparison(experiments)

print("All graphs generated.")
