import subprocess

class sample:
	def __init__(self,sample,nJobs):
		self.sample=sample
		self.nJobs=nJobs
		self.currentJob=1
	def updateJobN(self):
		self.currentJob+=1
	def getCommand(self):
		self.command="/afs/cern.ch/user/j/jluetic/public/Zbb/CMSSW_7_4_11/src/DYJetsMY/runZJets_newformat doWhat="+self.sample+" nJobs="+str(self.nJobs)+" jobNum="+str(self.currentJob)+" mcYieldScale="+str(1./self.nJobs)
		return self.command

doSubmit = True
subprocess.call("make", shell=True)
#samples = {"BACKGROUND"}
#samples = {"DATA"}
#samples = {"DYJETS","DATA"}
samples = {"DYJETS","DATA","BACKGROUND"}
nJobs = 30
f = open("template.sh","r").read()

for s in samples:
	bla = sample(s,nJobs)
	for i in range(1,nJobs+1):
		print bla.getCommand()
		f_new = open("new_script.sh","w")
		f_new.write(f)
		f_new.write(bla.getCommand()+"\n")
		f_new.close()

		command = "bsub -q 1nh -o out < new_script.sh"
		print command
		if doSubmit:
			subprocess.call(command, shell=True)
		bla.updateJobN()

	nJobs-=10


