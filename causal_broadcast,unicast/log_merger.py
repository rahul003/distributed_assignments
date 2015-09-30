import os
import prettytable
from prettytable import PrettyTable
import sys

timeoffset = 2

mode = str(sys.argv[1])
num_process = 0
dirpath = os.path.join(os.getcwd(),"logs/")
fullog = {}
for filename in os.listdir(dirpath):
	# print filename
	f = open(os.path.join(dirpath, filename))
	num_process+=1
	timeset = None
	pid = filename[4:]
	for line in f:
		if line.strip():
			line = line.rstrip()
	 		if line[:4] == 'Time':
				timeset = line[6:]
				timeset = int(timeset)
			else:
				# print timeset, fullog
				try:
					fullog[timeset][pid].append(line)
				except:
					if timeset not in fullog:
						fullog[timeset] = {}
					fullog[timeset][pid] = [line]

	f.close()

# print fullog

newtable = ['Time']
for i in range(num_process):
	newtable.append('p_'+str(i))
t = PrettyTable(newtable)
t.hrules = prettytable.ALL
# f = open(os.path.join(dirpath, "merged"), 'w')
firsttime = True
for key in sorted(fullog):
	row = []
	time_key =int(key)-timeoffset
	if time_key and firsttime:
		for i in range(time_key):
			fakerow = [str(i)]
			for j in range(num_process):
				fakerow.append('-')
			t.add_row(fakerow)
		firsttime = False

	row.append(str(time_key))
	for i in range(num_process):
		try:
			proc_log = fullog[key][str(i)]
			# print proc_log
			proc_lg_str = ''
			for l in proc_log:
				proc_lg_str += l
				proc_lg_str += '\n'
			row.append(proc_lg_str.rstrip())
		except:
			row.append(' - ')
	t.add_row(row)

with open(os.path.join(os.getcwd(), 'merged_'+mode+'.log'), 'w') as ofile:
	ofile.write(t.get_string())
