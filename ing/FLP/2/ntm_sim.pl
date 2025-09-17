/**
 * @ Author: Ondřej Koumar
 * @ Project: FLP 2024/2025 - Nondeterministic Turing Machine simulator
 * @ Create Date: 2025-04-22
 */

% Transitional rules are dynamically allocated in the predicate database.
:- dynamic rule/4.

/**
 * main/0
 * The program entry point. Reads and parses the input, runs the simulation.
 */
main :-
    retractall(rule(_,_,_,_)),
    read_input(RulesStrings, TapeString),
    parse_and_assert_rules(RulesStrings),
    string_chars(TapeString, TapeChars),
    build_initial_conf(TapeChars, InitialConf),
    ( simulate(InitialConf, []) ->
        true
    ;
        writeln('Error: No path found to the final state.'),
        halt(1)
    ).

/**
 * read_input(-RulesStrings:list, -TapeString:string)
 * Read all lines from standard input. Interpret the last one as a tape (TapeString),
 * others as transition rules (RulesString).
 */
read_input(RulesStrings, TapeString) :-
    read_lines_to_list(AllLines),
    ( AllLines = [] ->
        RulesStrings = [],
        TapeString = ""
    ; AllLines = [_|_] ->
        % First reverse - move tape string to the head position
        reverse(AllLines, [TapeString | RevRulesStrings]),
        % After getting tape string, reverse the reversed rules back.
        reverse(RevRulesStrings, RulesStrings)
    ).

/**
 * read_lines_to_list(-Lines:list)
 * Read lines one by one from stdin and insert it in Lines.
 */
read_lines_to_list(Lines) :-
    % user_input, end_of_file - aliases for stdin and EOF, respectively.
    read_line_to_string(user_input, Line),
    ( Line == end_of_file ->
        Lines = []
    ;
        Lines = [Line | RestLines],
        read_lines_to_list(RestLines)
    ).

/**
 * parse_and_assert_rules(+RulesStrings:list)
 * Parse the list of rules from standard stdin. Add each rule to dynamic
 * predicate database. If there are only three symbols on each line, the 
 * rule is considered an epsilon-rule.
 */
parse_and_assert_rules([]).
parse_and_assert_rules([RuleString | Rest]) :-
    normalize_space(string(RuleStrNorm), RuleString),
    split_string(RuleStrNorm, " ", "", Parts),
    (
        % Try to construct a 'full-defined' rule
        Parts = [StateStr, SymbolStr, NewStateStr, ActionStr],
        atom_string(State, StateStr), atom_string(SymbolAtom, SymbolStr),
        ( SymbolAtom == ' ' ->
            Symbol = ' ' 
        ; 
            (atom_length(SymbolAtom, 1) ->
                Symbol = SymbolAtom 
            ;
                fail) ),
        atom_string(NewState, NewStateStr), parse_action(ActionStr, Action),
        assertz(rule(State, Symbol, NewState, Action))
    ;
        % Try to construct an epsilon-rule
        Parts = [StateStr, NewStateStr, ActionStr],
        atom_string(State, StateStr), atom_string(NewState, NewStateStr),
        parse_action(ActionStr, Action),
        assertz(rule(State, epsilon, NewState, Action))
    ;
        % None of the above could be constructed; error.
        format(string(Err), 'Error: Invalid rule format: "~s"', [RuleString]),
        writeln(Err), halt(3)
    ),
    parse_and_assert_rules(Rest).

/**
 * parse_action(+ActionStr:string, -Action:atom)
 * Convert ActionStr to a character atom and check if it's a valid action to perform.
 */
parse_action(ActionStr, ActionAtom) :-
    atom_string(ActionAtom, ActionStr),         
    ( memberchk(ActionAtom, ['L', 'R', ' ']) 
    ; atom_length(ActionAtom, 1)                 
    ).

/**
 * build_initial_conf(+TapeChars:list, -InitialConf:term)
 * Create an initial configuration of the Turing machine from parsed tape.
 * Build a correct configuration for empty tape.
 */
build_initial_conf([], conf([], 'S', ' ', [])).
build_initial_conf([H|T], conf([], 'S', H, T)).

/**
 * simulate(+Conf:term, +Visited:list)
 * The main loop performing the Turing machine simulation.
 * Loop detection is done by adding all the used configurations to Visited list.
 * If the TM is in a configuration for a second time, program halts.
 */
simulate(Conf, Visited) :-
    ( member(Conf, Visited) ->
        print_conf(Conf),
        writeln('Error: Loop Detected.'),
        halt(2)
    ;
        print_conf(Conf),
        Conf = conf(_, State, _, _),
        ( State == 'F' ->
            true
        ;
            ( step(Conf, NextConf) ->
                simulate(NextConf, [Conf | Visited])
            ;
                fail
            )
        )
    ).

/**
 * step(+CurrentConf:term, -NextConf:term)
 * Perform one simulation step. Find a suitable rule for current configuration and apply it.
 */
step(conf(TapeBeforeRev, State, SymbolUnder, TapeAfter), NextConf) :-
    find_prioritized_rule(State, SymbolUnder, NewState, Action),
    apply_action(Action, NewState, TapeBeforeRev, SymbolUnder, TapeAfter, NextConf).

/**
 * find_prioritized_rule(+State:atom, +SymbolUnder:atom, -NewState:atom, -Action:atom)
 * Find a rule for the current computation step. Prioritize rules that lead to
 * the final state, whether it's epsilon or non-epsilon rules.
 * This is a little optimization which helps to get to the final state faster.
 */
find_prioritized_rule(State, SymbolUnder, NewState, Action) :-
    ( rule(State, SymbolUnder, 'F', Action), !, NewState = 'F'
    ; rule(State, epsilon, 'F', Action), !, NewState = 'F'
    ; rule(State, SymbolUnder, NewState, Action)
    ; rule(State, epsilon, NewState, Action)
    ).

/**
 * apply_action(+Action:atom, +NewState:atom, +TapeBeforeRev:atom, +SymbolUnder:atom, +TapeAfter:atom -NextConf:term)
 * Create a new configuration for the Turing machine based on the new values from used rule.
 */
apply_action(NewSymbol, NewState, TapeBeforeRev, _, TapeAfter, conf(TapeBeforeRev, NewState, NewSymbol, TapeAfter)) :-
    atom(NewSymbol), NewSymbol \= 'L', NewSymbol \= 'R', !.

apply_action('R', NewState, TapeBeforeRev, SymbolUnder, [], conf([SymbolUnder | TapeBeforeRev], NewState, ' ', [])) :- !.
apply_action('R', NewState, TapeBeforeRev, SymbolUnder, [NextSymbol | RestOfTapeAfter], conf([SymbolUnder | TapeBeforeRev], NewState, NextSymbol, RestOfTapeAfter)).

% Turing machine is on the beggining of the tape and wants to make a move left -> error.
apply_action('L', _, [], _, _, _) :-
    writeln('Error: Attempted to move left from the leftmost tape position.'),
    halt(5).
apply_action('L', NewState, [PrevSymbol | RestOfTapeBeforeRev], SymbolUnder, TapeAfter, conf(RestOfTapeBeforeRev, NewState, PrevSymbol, [SymbolUnder | TapeAfter])).

% This predicate should never be a target with such arguments, though it was useful while debugging the program.
% Keeping this here just for better readability of error messages.
apply_action(Action, NewState, _, _, _, _) :-
    format(string(Err), 'Internal error: Unhandled action type "~w" for state ~w.', [Action, NewState]),
    writeln(Err), halt(4).

/**
 * print_conf(+Conf:term)
 * Print the current configuration of the Turing Machine in format
 * tape before head, current state, symbol under the head, tape after head
 * with no separators (as one string).
 */
print_conf(conf(TapeBeforeRev, State, SymbolUnder, TapeAfter)) :-
    reverse(TapeBeforeRev, TapeBefore),
    ( TapeBefore == [] ->
        Pre = "" 
    ; 
        atomic_list_concat(TapeBefore, '', Pre) 
    ),
    ( TapeAfter == [] -> 
        Post = "" 
    ; 
        atomic_list_concat(TapeAfter, '', Post) 
    ),
    format('~w~w~w~w~n', [Pre, State, SymbolUnder, Post]).