#!/usr/bin/env groovy

pipeline {
	agent any
	stages {

		stage('\u2776 Build SqliteWrapped') {
			steps {								
				dir ('sqlitewrap') {        
					sh 'make -f Makefile clean'
					sh 'make -f Makefile -j3'
				}
			}
		}

		stage('\u2776 Build Xrm-Server') {
			 steps {								
				sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/src/+../src/+' linux/xrm-server.mk'''
				sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/src/+../src/+' linux/xrm-server.mk'''
				dir ('linux') {
					sh 'make -f Makefile clean'
					sh 'make -f Makefile -j3'
				}
			 }
		}
	}
}
