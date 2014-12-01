CC=ccsc
PK2=pk2cmd

DEVICE=PIC18F252
UNIT1=Calendario
UNIT1_FILE=calendario

#LIB_PATH="C:\PICC\Devices;C:\PICC\Drivers"

#HFLAGS=+FH +LN -T -A -M -Z +DF +Y=9 +STDOUT +EA I=$(LIB_PATH)
#BFLAGS=+FB +LN -T -A -M -Z +DF +Y=9 +STDOUT +EA I=$(LIB_PATH)
#MFLAGS=+FM +LN -T -A -M -Z +DF +Y=9 +STDOUT +EA I=$(LIB_PATH)
HFLAGS=+FH +LN -T -A -M -Z +DF +Y=9 +STDOUT +EA
PK2FLAGS=-E -P$(DEVICE) -M -R -J -F

all: $(UNIT1) $(UNIT2) $(UNIT3)

$(UNIT1): $(UNIT1_FILE).c
	$(CC) $(HFLAGS) $(UNIT1_FILE).c
	
Burn:
	$(PK2) $(PK2FLAGS) $(UNIT1_FILE).hex

clean:
	rm *.cof *.err *.esym *.hex *.lst *.pjt *.STA *.sym *.tre *.MCP *.PWI *.DBK