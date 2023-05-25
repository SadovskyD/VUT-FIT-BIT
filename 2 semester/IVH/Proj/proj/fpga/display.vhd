library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity display is
	port (
		reset : in std_logic;
		clk : in std_logic; -- hodiny 25 MHz
		smclk : in std_logic; -- hodiny 7.3728MHz
		data : std_logic_vector(127 downto 0);
		A : out std_logic_vector(3 downto 0);
		R : out std_logic_vector(7 downto 0)
	);
end entity display;

architecture behv of display is
   	
	function log2(A: integer) return integer is
		variable bits : integer := 0;
		variable b : integer := 1;
	begin
		while (b <= a) loop
			b := b * 2;
			bits := bits + 1;
		end loop;
		return bits;
	end function;
	
	constant TMR  : natural := 26000;
	constant AS   : natural := 10;
    
	signal cnt    : std_logic_vector(log2(TMR) downto 0) := (others => '0');
	signal A_temp : std_logic_vector(3 downto 0) := (others => '0');
	signal R_temp : std_logic_vector(7 downto 0) := (others => '0');
	signal en     : std_logic;
	signal an_en  :	std_logic;
	signal an_cnt : std_logic_vector(log2(AS) downto 0) := (others => '0');
	signal anim   : std_logic_vector (7 downto 0) := (others => '0');
	signal data_l : std_logic_vector(127 downto 0) := (others => '0');
begin

	counter: process(clk)
	begin
		if reset = '1' then
			cnt <= (others => '0');
		elsif ( rising_edge(clk) ) then
			cnt <= cnt + 1;
			en <= '0';
			if cnt = TMR then
				cnt <= (others => '0');
				en <= '1';
			end if;
		end if;
	end process;
	
	a_counter: process(en)
	begin
		if reset = '1' then
			an_cnt <= (others => '0');
		elsif ( A_temp = "1111" and (rising_edge(en)) ) then
			an_cnt <= an_cnt + 1;
			an_en <= '0';
			if an_cnt = AS then
				an_cnt <= (others => '0');
				an_en <= '1';
			end if;
		end if;
	end process;
	
	anm: process(clk, an_en)
	begin
			if (rising_edge(an_en)) then
				if ( CONV_INTEGER(anim) <= 48 and CONV_INTEGER(anim) >= 1 ) then
					data_l <= 
					data_l(126 downto 112)&data_l(127)&
					data_l(110 downto 96)&data_l(111)&
					data_l(94 downto 80)&data_l(95)&
					data_l(78 downto 64)&data_l(79)&
					data_l(62 downto 48)&data_l(63)&
					data_l(46 downto 32)&data_l(47)&
					data_l(30 downto 16)&data_l(31)&
					data_l(14 downto 0)&data_l(15);
				elsif (CONV_INTEGER(anim) <= 96 and CONV_INTEGER(anim) >= 48) then
					data_l <= 
					data_l(112)&data_l(127 downto 113)&
					data_l(96)&data_l(111 downto 97)&
					data_l(80)&data_l(95 downto 81)&
					data_l(64)&data_l(79 downto 65)&
					data_l(48)&data_l(63 downto 49)&
					data_l(32)&data_l(47 downto 33)&
					data_l(16)&data_l(31 downto 17)&
					data_l(0)&data_l(15 downto 1);
				elsif CONV_INTEGER(anim) = 97 then
					data_l <= "0000000000000000" &
							"0000000000000000" &
							"0000001000000000" &
							"0000011100000000" &
							"0000101010000000" &
							"0000001000000000" &
							"0000010100000000" &
							"0000010100000000";
				elsif (CONV_INTEGER(anim) = 101 or CONV_INTEGER(anim) = 109) then
					data_l(111 downto 0) <= data_l(95 downto 0)&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0));
				elsif (CONV_INTEGER(anim) = 105 or CONV_INTEGER(anim) = 120) then
					data_l(111 downto 0) <= (data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&data_l(111 downto 16);
				elsif CONV_INTEGER(anim) = 113 then
					data_l(127 downto 16) <= data_l(111 downto 16)&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0));
				elsif CONV_INTEGER(anim) = 114 then
					data_l <= not(data_l);
				elsif (CONV_INTEGER(anim) = 117) then
					data_l(127 downto 16) <= (data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&(data_l(0))&data_l(127 downto 32);
				elsif (CONV_INTEGER(anim) <=132 and CONV_INTEGER(anim) > 120) then
				data_l <= 
					"1"&data_l(127 downto 113)&
					"1"&data_l(111 downto 97)&
					"1"&data_l(95 downto 81)&
					"1"&data_l(79 downto 65)&
					"1"&data_l(63 downto 49)&
					"1"&data_l(47 downto 33)&
					"1"&data_l(31 downto 17)&
					"1"&data_l(15 downto 1);
				end if;
				anim <= anim + 1;
				if (CONV_INTEGER(anim) > 132 or CONV_INTEGER(anim) = 0) then
					data_l <= data;
					anim <= "00000001";
				end if;
			end if;
	end process;
	
	col: process(en)
		function GETID( X, Y, ROWS, COLS: integer) return integer is
			variable id : integer := 0;
		begin 
			id := (rows*(X mod COLS))+(Y mod ROWS); 
			return id;
		end function GETID;
		variable x : integer range 0 to 15 := 15;
	begin
		if reset = '1' then
		A_temp <= (others => '0');
		R_temp <= (others => '0');
		x := 15;
		elsif ( rising_edge(en) ) then
			R_temp(7) <= data_l(GETID(0, x, 16, 8));
			R_temp(6) <= data_l(GETID(1, x, 16, 8));
			R_temp(5) <= data_l(GETID(2, x, 16, 8));
			R_temp(4) <= data_l(GETID(3, x, 16, 8));
			R_temp(3) <= data_l(GETID(4, x, 16, 8));
			R_temp(2) <= data_l(GETID(5, x, 16, 8));
			R_temp(1) <= data_l(GETID(6, x, 16, 8));
			R_temp(0) <= data_l(GETID(7, x, 16, 8));
			x := x-1;
			A_temp <= A_temp +1;
		end if;
	end process;
	
	A <= A_temp-1;
	R <= R_temp;

end behv;

