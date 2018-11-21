results = read.csv("c_result.csv")
results_avg = aggregate( time ~ algorithm:matrix_size, data=results, FUN=mean)
results_avg$sd = aggregate( time ~ algorithm:matrix_size, data=results, FUN=sd)$time
results_avg = results_avg[order(results_avg$algorithm),]

c_naive_multiplication = results_avg[results_avg$algorithm=="c_naive_multiplication",]
c_GSL_multiplication = results_avg[results_avg$algorithm=="c_GSL_multiplication",]
c_better_multiplication = results_avg[results_avg$algorithm=="c_better_multiplication",]


c_naive_multiplication_fit = lm(time ~ poly(matrix_size, 3, raw=TRUE), data=c_naive_multiplication)
c_GSL_multiplication_fit = lm(time ~ poly(matrix_size, 3, raw=TRUE), data=c_GSL_multiplication)
c_better_multiplication_fit = lm(time ~ poly(matrix_size, 3, raw=TRUE), data=c_better_multiplication)


c_naive_line = data.frame(matrix_size = seq(600,1800, by=20))
c_naive_line$time = predict(c_naive_multiplication_fit,c_naive_line)
c_naive_line$algorithm = "c_naive_multiplication"

c_GSL_line = data.frame(matrix_size = seq(600,1800, by=20))
c_GSL_line$time = predict(c_GSL_multiplication_fit,c_GSL_line)
c_GSL_line$algorithm = "c_GSL_multiplication"

c_better_line = data.frame(matrix_size = seq(600,1800, by=20))
c_better_line$time = predict(c_better_multiplication_fit,c_better_line)
c_better_line$algorithm = "c_better_multiplication"

library(ggplot2)
ggplot(results_avg, aes(x=matrix_size, y=time, colour=algorithm, group=algorithm)) +
  geom_errorbar(aes(ymin=time-sd, ymax=time+sd, group=algorithm), colour="black", width=0.25) +
  geom_point() +
  ggtitle("Matrix multiplication in different algorithms") + 
  geom_line(data=c_native_line, aes(x=matrix_size,y=time)) + 
  geom_line(data=c_GSL_line, aes(matrix_size,time)) + 
  geom_line(data=c_better_line, aes(matrix_size,time))
