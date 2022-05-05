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
		stage('\u2777 Build MenuConvert') {
			steps {
				dir ('linux') {
					sh 'make -f xrm-menu-convert.mk -j2'
				}
			}
		}
		stage('\u2778 Build PromptConvert') {
			steps {
				dir ('linux') {
					sh 'make -f xrm-menu-prompt-convert.mk -j2'
				}
			}
		}
		stage('\u2779 Build UnitTests') {
			steps {
				dir ('linux') {
					sh 'make -f xrm-unittest.mk -j2'
				}
			}
		}
		stage('\u277A Run UnitTests') {
			steps {
				dir ('linux') {
					echo 'Running Unit Tests...'
					dir ('Debug') {
						sh './xrm-unittest'
					}
				}
			}
		}
		stage('\u277B Build Appplication') {
			 steps {
				dir ('linux') {
					sh 'make -f xrm-server.mk -j2'
				}
			}
		}
	}
}
