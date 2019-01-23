all: generate
	
cleanDataSet:
	rm -r DataSets/*

generate:
	python generateDataSet.py

regenerate: 
	python generateDataSet.py -force

simul: 
	python generateDataSet.py -simul

help: 
	python generateDataSet.py -help

listing: 
	python generateDataSet.py -list

unitTest:
	python generateDataSet.py -unitTest

exampleUse:
	python exampleUse.py UnitTestOut/001/001/

