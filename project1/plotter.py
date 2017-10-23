import pylab as plt
import numpy

filename = 'avg_output'
dump = open(filename, 'rt')
d=numpy.loadtxt(dump,delimiter=',', dtype=numpy.float)

lmbda=[]
expectAvg=[]
theoAvg=[]
expectTime=[]
theoTime=[]
expectBlock=[]
theoBlock=[]
for data in d:
    print(data)
    lmbda.append(data[0])
    expectAvg.append(data[1])
    theoAvg.append(data[2])
    expectTime.append(data[3])
    theoTime.append(data[4])
    expectBlock.append(data[5])
    theoBlock.append(data[6])

plt.title("Avg Number vs Lambda")
plt.plot(lmbda, expectAvg, 'r-', label= 'expected avg number')
plt.plot(lmbda, theoAvg, 'b-', label= 'theoritical avg number')
plt.legend(loc='upper left')
plt.show()

plt.title("Expected Time in system vs Lambda")
plt.plot(lmbda, expectTime, 'r-', label= 'expected time in system')
plt.plot(lmbda, theoTime, 'b-', label= 'theoritical time in system')
plt.legend(loc='upper left')
plt.show()

plt.title("blocking vs Lambda")
plt.plot(lmbda, expectBlock, 'r-', label= 'blocking probability of system')
plt.plot(lmbda, theoBlock, 'b-', label= 'theoritical blocking probability of system')
plt.legend(loc='upper left')
plt.show()
