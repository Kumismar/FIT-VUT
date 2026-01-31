import time
import argparse
from itertools import permutations
from pysat.solvers import Minisat22


# ---------- CNF Writer ----------
class CNFWriter:
    """Helper class for building CNF formulas."""

    def __init__(self):
        self.var_count = 0
        self.clauses = []

    def new_var(self):
        """Create a new variable and return its ID."""

        self.var_count += 1
        return self.var_count

    def add_clause(self, *lits):
        """Add a clause to the formula."""

        self.clauses.append(lits)


# ---------- Helper functions for preferences ----------
def generate_all_preference_orders(num_alternatives):
    """
    Generate all possible preference orders over alternatives.
    """

    return list(permutations(range(num_alternatives)))


def create_preference_matrix(preference_order, num_alternatives):
    """
    Convert a preference order to a binary preference matrix.
    matrix[a][b] = True means "a is preferred to b".
    """

    matrix = [[False] * num_alternatives for _ in range(num_alternatives)]

    # Map each alternative to its position in the order
    position = {}
    for pos, alt in enumerate(preference_order):
        position[alt] = pos

    # Alternative a is preferred to b if it has a better (lower) position
    for a in range(num_alternatives):
        for b in range(num_alternatives):
            if position[a] < position[b]:
                matrix[a][b] = True

    return matrix


def generate_all_preference_profiles(num_voters, all_preferences):
    """
    Generate all possible preference profiles.

    A profile assigns a preference order to each voter.
    For n voters and k preference types, we get k^n profiles.
    Returns list of profiles where each profile is [pref_idx_voter1, pref_idx_voter2, ...].
    """

    num_preference_types = len(all_preferences)

    # Recursively generate all combinations
    def generate_profiles(voter_idx):
        if voter_idx == num_voters:
            return [[]]

        sub_profiles = generate_profiles(voter_idx + 1)
        result = []
        for pref_idx in range(num_preference_types):
            for sub_profile in sub_profiles:
                result.append([pref_idx] + sub_profile)
        return result

    return generate_profiles(0)


# ---------- Encoding Arrow's Theorem ----------
def encode_arrow(num_voters, num_alternatives, use_pareto, use_iia, use_non_dictator):
    """
    Encode Arrow's Impossibility Theorem as CNF.
    """

    t0 = time.time()

    # Generate all possible preferences and profiles
    all_preference_orders = generate_all_preference_orders(num_alternatives)

    # Create preference matrices for each preference order
    preference_matrices = []
    for pref_order in all_preference_orders:
        matrix = create_preference_matrix(pref_order, num_alternatives)
        preference_matrices.append(matrix)

    # Generate all preference profiles
    profiles = generate_all_preference_profiles(
        num_voters, all_preference_orders)
    num_profiles = len(profiles)

    print(f"Number of profiles: {num_profiles}")

    # Create CNF and variables
    cnf_writer = CNFWriter()
    var_mapping = {}

    # For each profile and pair of alternatives, create a variable
    # var[(profile_idx, a, b)] = "society prefers a over b in profile profile_idx"
    for profile_idx in range(num_profiles):
        for a in range(num_alternatives):
            for b in range(a + 1, num_alternatives):
                var_mapping[(profile_idx, a, b)] = cnf_writer.new_var()

    def get_preference_literal(profile_idx, a, b):
        """
        Return literal representing "a is preferred to b in profile profile_idx".
        """

        if a < b:
            return var_mapping[(profile_idx, a, b)]
        else:
            return -var_mapping[(profile_idx, b, a)]

    # Always encode total order
    encode_total_order_constraints(cnf_writer, num_profiles, num_alternatives,
                                   get_preference_literal)

    # Optionally encode Pareto
    if use_pareto:
        encode_pareto_constraints(cnf_writer, num_alternatives,
                                  profiles, preference_matrices, get_preference_literal)

    # Optionally encode IIA
    if use_iia:
        encode_iia_constraints(cnf_writer, num_alternatives,
                               profiles, preference_matrices, get_preference_literal)

    # Optionally encode non-dictatorship
    if use_non_dictator:
        encode_non_dictatorship(cnf_writer, num_voters, num_alternatives,
                                profiles, preference_matrices, get_preference_literal)

    gen_time = time.time() - t0
    print(f"Generation time + encoding: {gen_time:.4f} s")
    print(f"Number of clauses: {len(cnf_writer.clauses)}")

    # Print which axioms are enabled
    axioms = []
    if use_pareto:
        axioms.append("Pareto")
    if use_iia:
        axioms.append("IIA")
    if use_non_dictator:
        axioms.append("NonDict")
    print(f"Axioms: {','.join(axioms) if axioms else 'None'}")

    return cnf_writer, var_mapping, profiles, preference_matrices


def encode_total_order_constraints(cnf_writer, num_profiles, num_alternatives, W):
    """
    Encode constraint that social choice must be a total order.

    Total order requires:
    1. Completeness: for all a,b either a>b or b>a
    2. Antisymmetry: not both a>b and b>a
    3. Transitivity: if a>b and b>c then a>c
    """

    for profile_idx in range(num_profiles):
        # Completeness and antisymmetry
        for a in range(num_alternatives):
            for b in range(a + 1, num_alternatives):
                # at least one
                cnf_writer.add_clause(W(profile_idx, a, b), W(
                    profile_idx, b, a))
                # at most one
                cnf_writer.add_clause(-W(profile_idx, a,
                                      b), -W(profile_idx, b, a))

        # Transitivity
        for a in range(num_alternatives):
            for b in range(num_alternatives):
                for c in range(num_alternatives):
                    if a == b or b == c or a == c:
                        continue
                    # a>b, b>c -> a>c
                    cnf_writer.add_clause(-W(profile_idx, a, b),
                                          -W(profile_idx, b, c),
                                          W(profile_idx, a, c))
                    # c>b, b>a -> c>a
                    cnf_writer.add_clause(-W(profile_idx, c, b),
                                          -W(profile_idx, b, a),
                                          W(profile_idx, c, a))


def encode_pareto_constraints(cnf_writer, num_alternatives, profiles, preference_matrices, W):
    """
    Encode Pareto efficiency to clauses.

    If all voters prefer a over b, then the social choice must also prefer a over b.
    """

    for a in range(num_alternatives):
        for b in range(num_alternatives):
            if a == b:
                continue

            # Check if all voters prefer a over b
            for profile_idx, profile in enumerate(profiles):
                all_prefer_a_over_b = True

                for voter_idx in range(len(profile)):
                    voter_preference_idx = profile[voter_idx]
                    voter_preferences = preference_matrices[voter_preference_idx]

                    if not voter_preferences[a][b]:
                        all_prefer_a_over_b = False
                        break

                # If all prefer a over b, society must prefer a over b
                if all_prefer_a_over_b:
                    cnf_writer.add_clause(W(profile_idx, a, b))


def encode_iia_constraints(cnf_writer, num_alternatives, profiles, preference_matrices, W):
    """
    Encode independence of irrelevant alternatives to clauses.

    Social choice between a and b should depend only on voters' relative
    preferences between a and b, not on preferences towards other alternatives.
    """

    for a in range(num_alternatives):
        for b in range(num_alternatives):
            if a == b:
                continue

            # All the profiles that got the same preferences between a and b belong to the same equivalence class
            equivalence_classes = {}

            for profile_idx, profile in enumerate(profiles):
                # Signature is just a fancy word for list of bools representing voters' preference between a and b.
                # For example, [True, True, False] means that voter 1: a>b, voter 2: a>b, voter 3: b>a in the profile.
                # Profiles with the same signature belong to the same equivalence class.
                signature = []
                for voter_idx in range(len(profile)):
                    voter_preference_idx = profile[voter_idx]
                    voter_preferences = preference_matrices[voter_preference_idx]
                    signature.append(voter_preferences[a][b])

                signature_tuple = tuple(signature)

                if signature_tuple not in equivalence_classes:
                    equivalence_classes[signature_tuple] = []
                equivalence_classes[signature_tuple].append(profile_idx)

            # All profiles within the same equivalence class have the same social choice
            for profile_indices in equivalence_classes.values():
                for i in range(len(profile_indices)):
                    for j in range(i + 1, len(profile_indices)):
                        profile1 = profile_indices[i]
                        profile2 = profile_indices[j]

                        # If profile1 prefers a over b, then profile2 must too
                        cnf_writer.add_clause(-W(profile1, a, b),
                                              W(profile2, a, b))

                        # If profile2 prefers a over b, then profile1 must too
                        cnf_writer.add_clause(
                            W(profile1, a, b), -W(profile2, a, b))


def encode_non_dictatorship(cnf_writer, num_voters, num_alternatives,
                            profiles, preference_matrices, W):
    """
    Encode non-dictatorship constraint to clauses.
    """

    for dictator_idx in range(num_voters):
        literals = []

        for profile_idx, profile in enumerate(profiles):
            dictator_preference_idx = profile[dictator_idx]
            dictator_preferences = preference_matrices[dictator_preference_idx]

            for a in range(num_alternatives):
                for b in range(num_alternatives):
                    if a == b:
                        continue

                    if dictator_preferences[a][b]:
                        literals.append(-W(profile_idx, a, b))

        # at least one of the literals must be true
        cnf_writer.add_clause(*literals)


# ---------- SAT Solving ----------
def run_solver(cnf):
    """Run SAT solver on the CNF formula using PySAT."""
    # Create solver instance
    solver = Minisat22()

    # Add all clauses to the solver
    for clause in cnf.clauses:
        solver.add_clause(clause)

    # Solve
    t0 = time.time()
    is_sat = solver.solve()
    solve_time = time.time() - t0

    if not is_sat:
        print("Result: unsat")
        print(f"Solving time: {solve_time:.4f} s")
        solver.delete()
        return None

    print("Result: sat")
    print(f"Solving time: {solve_time:.4f} s")

    # Get model
    model = solver.get_model()
    solver.delete()

    return model


def find_dictator(model, var_mapping, num_voters, num_alternatives, profiles, preference_matrices):
    """
    Find which voter is a dictator in the given model.

    A voter is a dictator if for all profiles and all pairs of alternatives,
    whenever the voter prefers a over b, the social choice also prefers a over b.
    """
    # Convert model (list of literals) to a set for fast lookup
    model_set = set(model)

    def is_true_in_model(var):
        """Check if a variable is true in the model."""
        return var in model_set

    def get_social_choice(profile_idx, a, b):
        """Get social choice preference between a and b in given profile."""
        if a < b:
            var = var_mapping[(profile_idx, a, b)]
            # True means a > b
            return is_true_in_model(var)
        else:
            var = var_mapping[(profile_idx, b, a)]
            # True means b > a
            return not is_true_in_model(var)

    # Check each voter to see if they're a dictator
    for voter_idx in range(num_voters):
        is_dictator = True

        # Check all profiles and all pairs of alternatives
        for profile_idx, profile in enumerate(profiles):
            voter_preference_idx = profile[voter_idx]
            voter_preferences = preference_matrices[voter_preference_idx]

            for a in range(num_alternatives):
                for b in range(num_alternatives):
                    if a == b:
                        continue

                    # Does voter prefer a over b?
                    voter_prefers_a_over_b = voter_preferences[a][b]

                    # Does social choice prefer a over b?
                    social_prefers_a_over_b = get_social_choice(
                        profile_idx, a, b)

                    # If voter prefers a over b but social choice doesn't, not a dictator
                    if voter_prefers_a_over_b and not social_prefers_a_over_b:
                        is_dictator = False
                        break

                if not is_dictator:
                    break

            if not is_dictator:
                break

        if is_dictator:
            return voter_idx

    return None


# ---------- Main ----------
if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Encode Arrow's Impossibility Theorem as a SAT problem"
    )
    parser.add_argument("--voters", type=int, default=2,
                        help="Number of voters")
    parser.add_argument("--alternatives", type=int, default=3,
                        help="Number of alternatives")
    parser.add_argument("--no-pareto", action="store_true",
                        help="Disable Pareto efficiency constraint")
    parser.add_argument("--no-iia", action="store_true",
                        help="Disable IIA constraint")
    parser.add_argument("--no-dictator", action="store_true",
                        help="Disable non-dictatorship constraint")
    args = parser.parse_args()

    # Determine which axioms are enabled
    use_pareto = not args.no_pareto
    use_iia = not args.no_iia
    use_non_dictator = not args.no_dictator

    # Generate CNF
    cnf, var_mapping, profiles, preference_matrices = encode_arrow(
        args.voters, args.alternatives, use_pareto, use_iia, use_non_dictator
    )

    # Run solver and get model
    model = run_solver(cnf)

    if model and not use_non_dictator:
        # Find which voter is the dictator
        dictator = find_dictator(model, var_mapping, args.voters, args.alternatives,
                                 profiles, preference_matrices)

        if dictator is not None:
            print(f"\nDictator: voter {dictator}")
