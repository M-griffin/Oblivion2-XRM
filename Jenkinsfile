#!/usr/bin/env groovy

pipeline {
	agent any
	stages {
		stage('\u2776 Build SqliteWrapped') {
			steps {
				dir ('linux') {
					sh 'make -f SqliteWrapped.mk clean'
					sh 'make -f SqliteWrapped.mk -j2'
					sh 'cat ../sqlitewrap/IError.h ../sqlitewrap/StderrLog.h ../sqlitewrap/SysLogs.h ../sqlitewrap/Database.h ../sqlitewrap/Query.h > ./Debug/libSqliteWrapped.h'
				}
			}
		}
		stage('\u2777 Build Xrm-Menu-Convert') {
			steps {
				dir ('linux') {
					sh 'make -f xrm-menu-convert.mk -j2'
				}
			}
		}
		stage('\u2778 Build Xrm-Menu-Prompt-Convert') {
			steps {
				dir ('linux') {
					sh 'make -f xrm-menu-prompt-convert.mk -j2'
				}
			}
		}
		stage('\u2779 Build Xrm-UnitTests') {
			steps {
				dir ('linux') {
					sh 'make -f xrm-unittest.mk -j2'

					// Run tests
					echo 'Running Unit Tests...'
					dir ('Debug') {
						sh './xrm-unittest'
					}
				}
			}
		}
		stage('\u277A Build Xrm-Server') {
			 steps {
				dir ('linux') {
					sh 'make -f xrm-server.mk -j2'
				}
			}
		}
	}
}
