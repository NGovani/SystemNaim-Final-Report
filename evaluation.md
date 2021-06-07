# Plan
- Start with varying the spi CLK rate until we can reach the highest
- Second show the performance across 3 different setups, single thread, multi-thread single chip, multi thread multi chip.
- Finally do a qualitative comparison

# SPI CLK
## Objectives
- Find the maximum clock rate we can use the SPI core

## Discussion
- Do some maths show that the latency of the system is dependent on the size of the program plus the overhead.
- The overhead is divided into two parts, one part comes from the SystemNaim Interconnect which is likely static across all clock speed, the second is from the channel bandwidth.
- For the highest performance we would like to maximise the channel bandwidth without losing performance

## Results

- Use same program. Two sub functions with loops to have some measurable speed.
- Single thread, and multi-thread single chip won't vary in latency so only run them once.
- MTSC will show up the latency of the program, and any additional latency comes from the overhead of doing a remote call.
- Estimate the latency of the interconnect, since we have a known channel bandwidth.
- Find subfunction latency to get more accurate Interconnect Overhead

ST: 212 
MTSC: 124

| SPI Clock (Hz) | Channel Bandwidth(Cycles/Bit) | Latency (Cycles) | Overhead (MTSC Latency - Latency) | Est. Transfer Overhead (Cycles)  | Est. Interconnect Overhead (Cycles)|
| ---            | ---                           | ---              | ----------------------            |    -------------                 | ------------                       |
| 125,000        | 400                           | 52,918           | 52,794                            |  51,200                          | 1,594                              |
| 250,000        | 200                           | 26,518           | 26,394                            |  25,600                          | 794                                |
| 252,525*       | 198                           | 26,254           | 26,130                            |  25,344                          | 786                                |
| 277,777*       | 180                           | 23,878           | 23,754                            |  23,040                          | 714                                |
| 312,500        | 160                           | 26,532           | 26,408                            |  20,480                          | 5,928                              |
| 347,222*       | 144                           | 23,892           | 23,768                            |  18,432                          | 5,336                              |
| 500,000        | 100                           | 16,632           | 16,508                            |  12,800                          | 3,708                              |
| 1,000,000      | 50                            | 8,382            | 8,258                             |  6,400                           | 1,858                              |
| 1,923,076*     | 26                            | 4,422            | 4,298                             |  3,328                           | 970                                |
| 5,000,000      | 10                            | 1,782            | 1,558                             |  1,280                           | 278                                |
| 8,333,333*     | 6                             | 1,122            | 998                               |  768                             | 230                                |
| 12,500,000     | 4                             | 792              | 668                               |  512                             | 156                                |
| 25,000,000     | 2                             | 542              | 418                               |  256                             | 162                                |



* Not exact divisors of the main system clock, this leads to some instability since these frequencies are derived from dividing the main clock
At 12,500,000 Hz the system starts to sometimes fail to complete transactions between the two devices.

## Conclusions
- Latency goes does as SPI Clock increases (Expected)
- This is mainly because the transfer overhead will decrease
- However at some points, the interconnect overhead will become very large. This is because of how the interconnect works.
    - While it is waiting for the data to return from the child fpga, it constantly polls the child fpga since it must initiate an SPI transactions
    - The child fpga must store the result into the slave SPI core and wait until the master initiates a transactions.
    - Master is constantly initiating transactions which take 1000s of cycles to complete due to low SPI clock frequency. 
    - If the child fpga completes its execution just after a master starts a transaction, the master will have to wait until this transaction is over ( may take 1000s of cycles), and then start another transaction to grab the result data (more 1000s of transaction).
    - Thus in some cases the interconnect overhead becomes much larger than expected. 
    - The points at which this occurs change for different programs and SPI clock speeds.

# System Performance 
## Objectives  
- Run a simple program i.e. numerical integration, then run on different setups

## Discussion
- Talk a little about how going from sequential does have a cost since you have to sometimes modify the algorithm which can increase the latency per iteration
- Move on to talking about how we're going to compare MTSC latency to MTMC latency in order to gauge the additional over head added by the use of off-chip. Similar to the above section.
- The say that the aim/hypothesis for this experiment is that as the we increase the accuracy of the numerical integration, and thus the latency since we add more iterations per run, we reduce the relative contribution of the interconnect + data transfer. Therefore, making it near equal to the MTSC, this is good because we now have access to more resources.( Maybe say that if the function we were testing was very complicated and resource intensive potentially a max of two could be put onto the fpga.)


## Results 
- Implemented a numerical integration method (Composite Simpson) and created a parallel algorithm to split the work load over 2 concurrent modules.
- Ran the system on a Single Thread(ST), Multi-Thread Single Chip (MTSC), Multi-Thread Multi Chip (MTMC) and compare latencies.
- MTSC should be tiwce as fast as ST since there are two parallel function calls. 
- MTMC should be equal to MTSC plus a somewhat static overhead
- Won't be static so calculate the mean of this overhead. Should be centred around some value. That is the average overhead for this run.
- Plot graphs showing how many cycles the overhead takes up when running the program.


| Steps (n)  | ST Latency (Cycles) | MTSC Latency (Cycles) | MTMC Latency (Cycles) | Est. Overhead (Cycles) |
| ---        | ---                 | ---                   | --------------------  |    -------------       |
| 200        | 2,572               | 1,370                 | 2,839                 |  1,469                 |
| 400        | 5,072               | 2,670                 | 4,215                 |  1,545                 |
| 600        | 7,572               | 3,970                 | 5,591                 |  1,621                 |
| 800        | 10,072              | 5,270                 | 6,967                 |  1,697                 |
| 1000       | 12,572              | 6,570                 | 7,999                 |  1,429                 |
| 1200       | 15,072              | 7,870                 | 9,375                 |  1,505                 |
| 1400       | 17,572              | 9,170                 | 10,751                |  1,581                 |
| 1600       | 20,054              | 10,470                | 12,127                |  1,657                 |
| 1800       | 22,572              | 11,770                | 13,503                |  1,733                 |
| 2000       | 25,072              | 13,070                | 14,535                |  1,465                 |


## Conclusion
- As, n, increases so does the overall latency of the program. The overhead (in the MTMC case), however, does not increase. Resulting the in percentage contribution of the overhead to the overall latency to decrease.
- Thus, bringing it more in line with the MTSC latency. This is good because we don't incur that much of a latency penalty for using a multi-fpga system when programs are designed to run for longer.