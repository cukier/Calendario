#define rs PIN_E0
#define en PIN_E1
#define d4 PIN_D4
#define d5 PIN_D5
#define d6 PIN_D6
#define d7 PIN_D7

void lcd_cmd(int cmd) {
	output_bit(d4, bit_test(cmd, 0));
	output_bit(d5, bit_test(cmd, 1));
	output_bit(d6, bit_test(cmd, 2));
	output_bit(d7, bit_test(cmd, 3));
	output_high(en);
	output_low(en);
}

void lcd_envia_byte(short endereco, int dado) {
	output_low(rs);
	output_bit(rs, endereco);
	delay_us(100);
	output_low(en);
	lcd_cmd(dado >> 4);
	lcd_cmd(dado & 0x0f);
	delay_ms(1);
}

void lcd_pos_xy(int x, int y) {
	int endereco;
	if (y != 1)
		endereco = 0xc0;
	else
		endereco = 0x80;
	endereco += x - 1;
	lcd_envia_byte(0, endereco);
}

void lcd(char c) {
	switch (c) {
	case '\f':
		lcd_envia_byte(0, 1);     // 0,1
		delay_ms(2);
		break;
	case '\n':
	case '\r':
		lcd_pos_xy(1, 2);      // 1,2  COLUNA,LINHA
		break;
	case '\b':
		lcd_envia_byte(0, 0x10);
		break;
	default:
		lcd_envia_byte(1, c);
		break;
	}
}

void lcd_init() {

	int conta;

	output_low(rs);
	output_low(en);
	output_low(d4);
	output_low(d5);
	output_low(d6);
	output_low(d7);

	delay_ms(15);

	for (conta = 1; conta <= 3; conta++) {
		lcd_cmd(0x03);
		delay_ms(5);
	}
	lcd_cmd(0x02);           //escreve comando para interface de 4 vias de dados
	lcd_envia_byte(0, 0x28); //escreve comando informando lcd de 2 linhas (5x7) em 4bits
	lcd_envia_byte(0, 0x0c); //escreve comando para limpar todo o display
	lcd_envia_byte(0, 0x01); //escreve comando para ligar o display **com cursor piscante
	lcd_envia_byte(0, 0x06); //escreve comando para incrementar automaticamente á direita
	delay_ms(5);
}

