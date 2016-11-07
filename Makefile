.PHONY: clean All

All:
	@echo "----------Building project:[ RatMove - Debug ]----------"
	@"$(MAKE)" -f  "RatMove.mk"
clean:
	@echo "----------Cleaning project:[ RatMove - Debug ]----------"
	@"$(MAKE)" -f  "RatMove.mk" clean
