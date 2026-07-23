# Run Log - The Flaky Network

- **Profile A (A_mild):**
  - Command: `python3 run.py --profile profiles/A.json --delay_ms 41`
  - Playout Delay: 41 ms
  - Miss Rate: < 0.47% (Valid)
  - Bandwidth Overhead: 2.00× (under 2.0× limit)
  - Changes & Rationale: I added a duplicate packet sender in my code, to decrease error and missing rate, theoritcially sending 2*1500=3000 packets. But intially using my code, i was getting error rate 0.7% and bandwidth 2.05x, this means 2.05*1500 = 3075, i need to cut down 75 or more duplicate packets. I Decided to cut 80 duplicate packets for safety out of the 1500 duplicated i was sending, 1500/80 = 18.75 = 19, i used the modulo operator to skip every frame_count which is multiple of 19. 

- **Profile B (B_moderate):**
  - Command: `python3 run.py --profile profiles/B.json --delay_ms 81`
  - Playout Delay: 81 ms
  - Miss Rate: < 0.87% (Valid)
  - Bandwidth Overhead: 2.00×
  - Changes & Rationale: Adjusted the playout delay window to 81 ms to accommodate Profile B's 80 ms maximum network delay ceiling, preventing late-arriving packets from triggering false deadline misses.
