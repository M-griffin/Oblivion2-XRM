.PHONY: clean All

All:
	@echo "----------Building project:[ SqliteWrapped - Debug ]----------"
	@"$(MAKE)" -f  "SqliteWrapped.mk" PreBuild && "$(MAKE)" -f  "SqliteWrapped.mk" && "$(MAKE)" -f  "SqliteWrapped.mk" PostBuild
	@echo "----------Building project:[ xrm-menu-prompt-convert - Debug ]----------"
	@"$(MAKE)" -f  "xrm-menu-prompt-convert.mk" PreBuild && "$(MAKE)" -f  "xrm-menu-prompt-convert.mk"
	@echo "----------Building project:[ xrm-menu-convert - Debug ]----------"
	@"$(MAKE)" -f  "xrm-menu-convert.mk" PreBuild && "$(MAKE)" -f  "xrm-menu-convert.mk"
	@echo "----------Building project:[ xrm-unittest - Debug ]----------"
	@"$(MAKE)" -f  "xrm-unittest.mk" PreBuild && "$(MAKE)" -f  "xrm-unittest.mk"
	@echo "----------Building project:[ xrm-server - Debug ]----------"
	@"$(MAKE)" -f  "xrm-server.mk" PreBuild && "$(MAKE)" -f  "xrm-server.mk"
clean:
	@echo "----------Cleaning project:[ SqliteWrapped - Debug ]----------"
	@"$(MAKE)" -f  "SqliteWrapped.mk"  clean
	@echo "----------Cleaning project:[ xrm-menu-prompt-convert - Debug ]----------"
	@"$(MAKE)" -f  "xrm-menu-prompt-convert.mk"  clean
	@echo "----------Cleaning project:[ xrm-menu-convert - Debug ]----------"
	@"$(MAKE)" -f  "xrm-menu-convert.mk"  clean
	@echo "----------Cleaning project:[ xrm-unittest - Debug ]----------"
	@"$(MAKE)" -f  "xrm-unittest.mk"  clean
	@echo "----------Cleaning project:[ xrm-server - Debug ]----------"
	@"$(MAKE)" -f  "xrm-server.mk" clean
