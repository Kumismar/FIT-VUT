# ISA - Monitorování DHCP komunikace

Pro více informací vizte docs/manual.pdf.

## Struktura projektu
Zdrojové soubory jsou ve složce src/, mají oddělené header soubory, které jsou ve složce src/headers/.
Dokumentace (manual.pdf a linux manpage) se nachází ve složce docs/.
Složka tests/ obsahuje unit testy, jako testovací framework byl vybrán googletest.
V dhcp_files/ je Python skript pro generování .pcap souborů, ze kterých program může číst.

## Spuštění
Program lze spustit invokací make pro přeložení a make run pro spuštění binárky.
V souboru args.txt lze nastavit argumenty příkazové řádky.

Testy se přeloží spuštěním make test (musí být předtím přeložený program) a spustí voláním make runtest.