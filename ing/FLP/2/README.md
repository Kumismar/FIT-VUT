# FLP 2024/2025 projekt - simulátor nedeterministického Turingova stroje

Ondřej Koumar, xkouma02

## Reprezentace stavů TS

Konfigurace TS je reprezentována termem `conf(TapeBeforeRev, CurrentState, SymbolUnderHead, TapeAfter)`.

`TapeBeforeRev` je seznam reprezentující pásku před čtecí hlavou. 
Pro optimalizaci je tento seznam reverzován, aby při pohybu hlavy doleva/doprava se v konstantním čase mohly provést operace vyjmutí/vložení symbolů z/do seznamu.
Kdyby tento seznam reverzován nebyl, složitost by byla lineární.

`CurrentState`, `SymbolUnderHead`, `TapeAfter` reprezentují aktuální stav stroje, symbol pod čtecí hlavou a zbytek pásky za čtecí hlavou (potenciálně nekonečný, nereverzovaný).

## Postup pro nalezení řešení

Nejdříve jsou přečtena přechodová pravidla ze vstupního souboru.
Jejich korektnost je kontrolována v průběhu stavby interní reprezentace pravidel - ta jsou reprezentována termem `rule(State, Symbol, NewState, Action)`, který je dynamickým predikátem.
Postupně, během parsování, se tyto predikáty vkládají do interní databáze a jsou dále využívány při samotné simulaci stroje.
Program podporuje stavbu epsilon-pravidel, je možné vynechat druhý symbol v pravidle a místo něj nechat mezeru.
Nicméně, pokud symbol chybí na jiné pozici, program jej stále vyhodnotí jako pokus o epsilon-pravidlo a chování programu se nedá předpovědět (undefined behavior).

Následně je zkonstruována počáteční konfigurace a je spuštěna simulace.
Predikát `simulate/2` implementuje hlavní logiku simulace.
Nejdříve se podívá, zda už daná konfigurace se někdy objevila (prohledání seznamu `Visited`).
Pokud ano, stroj se bere jako zacyklený a simulace končí neúspěchem.
Jinak získá další konfiguraci (predikát `step/2`) a pokračuje v simulaci dalším krokem.
Mezitím na `stdout` vytiskne aktuální konfiguraci.
Pokud se stroj zrovna nachází v koncovém stavu, ukončí simulaci úspěchem.

Predikát `step/2` vybere pravidlo, které použije pro aktualizaci konfigurace a aplikuje jej.
Implementoval jsem optimalizaci/heuristiku, která prioritizuje přechody do koncového stavu, jsou-li nějaké k dispozici.
Predikát `apply_action/6` vezme akci, kterou TS má provést, starou konfiguraci a vrací konfiguraci novou.

Je využito vlastnosti backtrackingu prologu.
Pokud aktuální cesta nevede k řešení, vyzkouší se jiný přechod, je-li k dispozici.
Vracení stromem stavů může jít klidně o několik úrovní nahoru (až ke kořeni, pokud neexistuje sekvence stavů).
Přirozeně, stavový prostor je prohledáván pomocí DFS.
Stavový prostor je prořezáván díky detekci už navštívených konfigurací a prioritizaci přechodů, které vedou do koncového stavu.

## Návod k použití

Dle pokynů v zadání jsem napsal Makefile, který program přeloží a výsledný binární soubor nechá v kořenovém adresáři projektu, a to pouze voláním příkazu `make` bez určení cíle.

Rozhodl jsem si vytvořit sadu 11 malých testů, které testují velmi základní funkcionalitu projektu, jejich implementace je ve složce `tests/`.
Testována je detekce cyklu, pohyb na pásce doleva, když už to nejde, jeden test na špatný formát pravidel v souboru, stroj bez přechodu do koncového stavu.
Toto jsou testy, které mají skončit neúspěchem, což testy potvrzují.
Dále tam mám 6 testů na velmi jednoduchou funkcionalitu a nalezení řešení a 1 test, který vytvoří poměrně velký Turingův stroj a na vstupu má relativně velký řetězec.

Všechny vstupní soubory pro program (\*.in) mají k sobě příslušný soubor s očekávaným návratovým kódem programu (\*.rc).
Testovací skript `test.sh` potom spouští všechny tyto testy na čerstvě přeloženém programu.
Disclaimer - s testy mi dost pomohla umělá inteligence, především se vstupními soubory, které jsem si zkontroloval a použil.
Model, který jsem ke generování testů použil, je Gemini.
Samozřejmě jsem neopoužil copy-paste způsob, upravil jsem si vše tak, aby vstupy dávaly smysl a hodně věcí jsem si kontroloval, ale zdaleka ne vše je moje práce.
Testovací skript jsem si napsal sám.

## Rozšíření

V zadání není explicitně psáno nic o epsilon-pravidlech.
Nevím, jestli se to dá používat za plnohodnotné rozšíření, každopádně to zde zmíním.
Program umí přečíst epsilon-pravidla, která v porovnání se standardními prividly vypadají na vstupu následovně (vizte `simple_test_05.in`):
```
S a B R
B a B R
B   B c
B c C R
C   E R
E   F x
```
V tomto formátu se sestaví stroj, který umí pracovat s epsilon pravidly.
Pozor, musí chybět symbol na korektním místě, tedy v druhém sloupci.
Jinak se epsilon-pravidlo nesestaví korektně a stroj bude mít nedefinované chování.

## Omezení

Dle zadání je implementováno vše povinné a během testování řešení jsem na žádné omezení nepřišel.
