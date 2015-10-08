.PHONY: clean All

All:
	@echo "----------Building project:[ xrm-unittest - Debug ]----------"
	@cd "xrm-unittest" && $(MAKE) -f  "xrm-unittest.mk"
clean:
	@echo "----------Cleaning project:[ xrm-unittest - Debug ]----------"
	@cd "xrm-unittest" && $(MAKE) -f  "xrm-unittest.mk" clean
