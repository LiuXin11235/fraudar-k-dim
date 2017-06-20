no = open('no.txt', 'w')
wd = open('wd.txt', 'w')

g = {}
gg = []
def G(s):
	if s not in g:
		gg.append(s)
		g[s] = len(g)
	return g[s]

lines = 0
for l in open('event.csv').readlines():
	a = l.strip().split(',')

#	if a[2] == '':
#		continue
#	if a[3] == '':
#		continue
#	if a[5] == '':
#		continue
	a[2] = str(int(a[2]) / 60)
	for i in range(10):
		a[i] += chr(65 + i)
	lines += 1
	print >>no, G(a[0]), G(a[1]), G(a[2]), G(a[3]), G(a[4]), G(a[5]), G(a[6]), G(a[7]), G(a[8]), G(a[9])
print len(g), lines, 10

