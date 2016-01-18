CC=ccsc
PK2=pk2cmd

DEVICE=PIC18F252
UNIT1=Calendario
UNIT1_FILE=calendario

HFLAGS=+FH +LN -T -A -M -Z +DF +Y=9 +STDOUT +EA
BFLAGS=+FB +LN -T -A -M -Z +DF +Y=9 +STDOUT +EA
MFLAGS=+FM +LN -T -A -M -Z +DF +Y=9 +STDOUT +EA

GLOBALS=+GPRINT_SERIAL=""

PK2FLAGS=-E -P$(DEVICE) -M -R -J -F

all: $(UNIT1)

$(UNIT1): $(UNIT1_FILE).c
	$(CC) $(HFLAGS) $(GLOBALS) $(UNIT1_FILE).c
	
burn:
	$(PK2) $(PK2FLAGS) $(UNIT1_FILE).hex

clean:
	rm *.cof *.err *.esym *.hex *.lst *.pjt *.STA *.sym *.tre *.MCP *.PWI *.DBK