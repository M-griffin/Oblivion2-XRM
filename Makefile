.PHONY: clean All

All:
	@echo "----------Building project:[ xrm-server - Debug ]----------"
	@"$(MAKE)" -f  "xrm-server.mk"
clean:
	@echo "----------Cleaning project:[ xrm-server - Debug ]----------"
	@"$(MAKE)" -f  "xrm-server.mk" clean
