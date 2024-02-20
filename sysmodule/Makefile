CONTENTS_ID	:= 4200000000000123

all: sysmodule/sysmodule.nsp out

dist: sysmodule.zip

clean:
	$(MAKE) -C sysmodule clean
	rm -rf out
	rm -rf sysmodule.zip

sysmodule/sysmodule.nsp: .FORCE
	$(MAKE) -C sysmodule

out: sysmodule/sysmodule.nsp
	@rm -rf out
	@mkdir -p out
	@mkdir -p out/atmosphere/contents/$(CONTENTS_ID)
	@mkdir -p out/atmosphere/contents/$(CONTENTS_ID)/flags
	@cp sysmodule/sysmodule.nsp out/atmosphere/contents/$(CONTENTS_ID)/exefs.nsp
	@cp sysmodule/toolbox.json out/atmosphere/contents/$(CONTENTS_ID)/toolbox.json
	@touch out/atmosphere/contents/$(CONTENTS_ID)/flags/boot2.flag

sys-gdbstub.zip: out
	@echo creating $@
	@cd $<; zip -r ../$@ ./*; cd ../;

.FORCE:

.PHONY: all dist clean