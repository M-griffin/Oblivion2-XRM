#!/usr/bin/env groovy

pipeline {
	agent any
	stages {
		stage('\u2776 Build SqliteWrapped') {
			steps {								
				dir ('sqlitewrap') {        
					sh 'make -f Makefile clean'
					sh 'make -f Makefile -j2'
				}
			}
		}
		
		stage('\u2776 Build Xrm-Menu-Convert') {
			steps {								
				dir ('xrm-menu-convert') {        
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/xrm-menu-convert/src/+src/+' xrm-menu-convert.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/xrm-menu-convert/src/+src/+' xrm-menu-convert.mk'''
					
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/src/+../src/+' xrm-menu-convert.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/src/+../src/+' xrm-menu-convert.mk'''
					sh 'make -f xrm-menu-convert.mk clean'
					sh 'make -f xrm-menu-convert.mk -j2'
				}
			}
		}	
		
		stage('\u2776 Build Xrm-Menu-Prompt-Convert') {
			steps {								
				dir ('xrm-menu-prompt-convert') {        
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/xrm-menu-prompt-convert/src/+src/+' xrm-menu-prompt-convert.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/xrm-menu-prompt-convert/src/+src/+' xrm-menu-prompt-convert.mk'''
					
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/src/+../src/+' xrm-menu-prompt-convert.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/src/+../src/+' xrm-menu-prompt-convert.mk'''
					sh 'make -f xrm-menu-prompt-convert.mk clean'
					sh 'make -f xrm-menu-prompt-convert.mk -j2'
				}
			}
		}
		
		stage('\u2776 Build Xrm-UnitTests') {
			steps {								
				dir ('xrm-unittest') {        									
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/xrm-unittest/++' xrm-unittest.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/xrm-unittest/++' xrm-unittest.mk'''
					
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/src/+../src/+' xrm-unittest.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/src/+../src/+' xrm-unittest.mk'''
					sh 'make -f xrm-unittest.mk clean'
					sh 'make -f xrm-unittest.mk -j2'
					
					// Run tests
					echo 'Running Unit Tests...'
					dir ('Debug') {        
						sh './xrm-unittest'
					}
				}
			}
		}
		
		stage('\u2776 Build Xrm-Server') {
			 steps {												
				dir ('linux') {
					sh '''sed -i 's+/home/blue/code/Oblivion2-XRM/src/+../src/+' xrm-server.mk'''
					sh '''sed -i 's+/home/merc/code/Oblivion2-XRM/src/+../src/+' xrm-server.mk'''
					sh 'make -f xrm-server.mk clean'
					sh 'make -f xrm-server.mk -j2'
				}
			 }
		}
	}
}
