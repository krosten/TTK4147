# Hei big kriss

## TASK A

Busy wait på bare A. Alle målinger ga 7, maks 7. Null variasjon, veldig deilig.

Kjørte bare 10 tester her, så fordelingen er litt tynn:)

## TASK B

Busy wait på A+B+C.

| | snitt | maks |
|---|---|---|
| A | 28.3 | 729 |
| B | 18.6 | 722 |
| C | 11.2 | 72 |

**Hvem har lavest maks?**

C har lavest maks når vi tester alle men med bare A har A seff lavest, fordi den testes alene idk?


**Ser A, B og C like ut?**

Nei, fordi de sjekkes etter hverandre? ikek samtidig?

## TASK C

med interrupts

| | snitt | maks |
|---|---|---|
| A | 16.1 | 39 |
| B | 15.9 | 39 |
| C | 14.7 | 36 |

**Mot polling?**

Labere gj-snitt på A og B og maksen går mye mer ned. som tekn sett er det viktigste


**Har A, B og C samme responstid?**

cirka samme respons tid, 

## TASK D

masse testrer og skit

| | A | B | C |
|---|---|---|---|
| rask B (task C) | 16.1 | 15.9 | 14.7 |
|  dyr B | 68.9 | 128.6 | 24.7 |
| deferred + rask B | 24.1 | 24.6 | 26.6 |
| deferred + dyr B | 31.3 | 139.3 | 35.7 |

**Er deferred raskere eller tregere?**

deferred er slow

fordi isr bare setter flagg, så det blir på en måte ekstra steg?

**Hvem tåler dyr B best for A og C?**

deferred har mindre impact

## TASK E

Ikke gjort.
