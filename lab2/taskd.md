Hei big noa

Så task D er ganske kul og her vil det komme ascii-visualiseringer for å forklare.

La oss si vi har minneområdet som kan romme 32 elementer som foreløpig er ubrukt:
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

Og så oppretter vi en dynamisk liste med 2 elementer. Allokert plass i minnet er innenfor () som er lk cursed men aksepter det:
[(1, 2,) 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

Hvis den vokser med x2 hver gang vi fyller den utover allokerte plassen, og som regel plasseres det nye arrayet rett etter det gamle i minnet:
[1, 2, (1, 2, 3, 4,) 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

Og så vokser den igjen:
[1, 2, 1, 2, 3, 4, (1, 2, 3, 4, 5, 6, 7, 8), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

Og så vokser den igjen:
[1, 2, 1, 2, 3, 4, 1, 2, 3, 4, 5, 6, 7, 8, (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), 0, 0]
Nå har den ikke plass til å vokse mer, det er 2 plasser foran og 14 plasser bak som begge er <32):



La oss nå se på neste eksempelet hvor listen vil vokse med 1,5x hver gang den går utover allokerte minne:
[(1, 2,) 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

Den vokser:
[1, 2, (1, 2, 3,) 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

Og vokser (runder 4,5 ned til 4):
[1, 2, 1, 2, 3, (1, 2, 3, 4,) 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

og vokser (runder 7,5 ned til 7):
[1, 2, 1, 2, 3, 1, 2, 3, 4, (1, 2, 3, 4, 5, 6, 7,) 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

og vokser (runder 10,5 ned til 10):
[1, 2, 1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 4, 5, 6, 7, (1, 2, 3, 4, 5, 6, 7, 8, 9, 10,) 0, 0, 0, 0, 0, 0]

Nå har den ikke plass til å vokse mer, eller har den det? Det er 6 plasser foran og 16 plasser bak, 16 plasser!!!!.
[(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,) 7, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0, 0, 0, 0, 0]
Den får plass bak den forrige allokerte plassen til listen! 2x could never achieve this kind of power

Hilsen big kriss