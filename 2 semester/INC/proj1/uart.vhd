-- uart.vhd: UART controller - receiving part
-- Author(s): xsadov06
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(
    CLK: 	    in std_logic;
	RST: 	    in std_logic;
	DIN: 	    in std_logic;
	DOUT: 	    out std_logic_vector(7 downto 0);
	DOUT_VLD: 	out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
	signal postn: std_logic_vector(3 downto 0);
	signal cnt: std_logic_vector(2 downto 0);
	signal en8: std_logic;
	signal en16: std_logic;
	signal CNT_EN: std_logic;
begin

	cnt8: process(CLK, RST, CNT_EN)
	begin
		if ((RST = '1') or falling_edge(CNT_EN)) then
			cnt <= "000";
			en8 <= '0';
		elsif (rising_edge(CLK) and (CNT_EN = '1')) then
			cnt <= cnt + 1;
			if cnt="111" then
				en8 <= '1';
			else en8 <= '0';
			end if;
		end if;
	end process;
	 
	cnt16: process(RST, en8, CNT_EN)
	begin
		if ((RST = '1') or falling_edge(CNT_EN)) then
			en16 <= '0';
		elsif rising_edge(en8) then
			en16 <= not en16; 
		end if;
	end process;
	
  	readp: process(en16, DIN, CLK)
  	begin
  		if (rising_edge (en16)) then
  			case postn is
				when "1000" => DOUT(7)  <= DIN;
				when "0111" => DOUT(6)  <= DIN;
				when "0110" => DOUT(5)  <= DIN;
				when "0101" => DOUT(4)  <= DIN;
				when "0100" => DOUT(3)  <= DIN;
				when "0011" => DOUT(2)  <= DIN;
				when "0010" => DOUT(1)  <= DIN;
				when "0001" => DOUT(0)  <= DIN;
				when "1001" => DOUT_VLD <= DIN;
				when others => null;
			end case;
		elsif (rising_edge (CLK)) then
			DOUT_VLD <= '0';
  		end if;
  	end process;
	
--Instance ridiciho automatu -----------------------------------------
FSM_U : entity work.uart_fsm
port map(
	CLK     => CLK,
	RST	    => RST,
	
	--Input Signals
	en8     => en8,
	en16    => en16,
	DIN     => DIN,
	
	--Output signals
	CNT_EN  => CNT_EN,
	postn   => postn
);

end behavioral;
