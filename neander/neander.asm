0 <- 3 
3 <- 2 
4 <- 2 
6 <- 3 
5 <- 1
LDA 6
NOT
ADD 5
ADD 4
STA 4

LDA 3
ADD 4
STA 3

1 <- 255
2 <- 0
label0_start: 
LDA 0
JZ label0_end 
ADD 1
STA 0
LDA 2
ADD 3
STA 2
JMP label0_start 
label0_end: 
LDA 2
STA 0
30 <- 0
LDA 0
STA 30
 