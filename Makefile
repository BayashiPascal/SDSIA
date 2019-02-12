help: 
	python generateDataSet.py -help

cleanDataSet:
	rm -r DataSets/*

generate:
	python generateDataSet.py

regenerate: 
	python generateDataSet.py -force

simul: 
	python generateDataSet.py -simul

listing: 
	python generateDataSet.py -list

unitTest:
	python generateDataSet.py -unitTest

exampleUse:
	python exampleUse.py UnitTestOut/001/001/

