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
		
		stage('\u2776 Build xrm-menu-convert') {
			steps {								
				dir ('xrm-menu-convert') {        
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/xrm-menu-convert/src/+src/+' xrm-menu-convert.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/xrm-menu-convert/src/+src/+' xrm-menu-convert.mk'''
					sh 'make -f Makefile clean'
					sh 'make -f Makefile -j3'
				}
			}
		}		
		
		stage('\u2776 Build Xrm-Server') {
			 steps {												
				dir ('linux') {
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/src/+../src/+' xrm-server.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/src/+../src/+' xrm-server.mk'''
					sh 'make -f Makefile clean'
					sh 'make -f Makefile -j2'
				}
			 }
		}
	}
}
