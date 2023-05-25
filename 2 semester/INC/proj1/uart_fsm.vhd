-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): xsadov06
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_FSM is
port(
   CLK     : in std_logic;
   RST     : in std_logic;
   DIN     : in std_logic;
   en8     : in std_logic;
   en16    : in std_logic;
   CNT_EN  : out std_logic;
   postn   : out std_logic_vector (3 downto 0)
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
type t_state is (wtn, rdn0, rdn1, rdn2, rdn3, rdn4, rdn5, rdn6, rdn7, rdn8, rdn9);
signal pstate : t_state;
signal nstate : t_state;
begin

	process (CLK, RST)
	begin
		if (rst='1') then
			pstate <= wtn;
		elsif rising_edge (CLK) then
			pstate <= nstate;
		end if;
	end process;
	
	process (pstate, DIN, en8, en16, CLK)
	begin
		if ((DIN = '0') and (pstate = wtn)) then
			nstate <= rdn0;
		elsif ((DIN = '0') and (pstate = rdn0) and (rising_edge(en8))) then
			nstate <= rdn1;
		elsif ((DIN = '1') and (pstate = rdn0) and (rising_edge(en8))) then
			nstate <= wtn;
		elsif (rising_edge(en16)) then
			if (pstate = rdn1) then
				nstate <= rdn2;
			elsif (pstate = rdn2) then
				nstate <= rdn3;
			elsif (pstate = rdn3) then
				nstate <= rdn4;
			elsif (pstate = rdn4) then
				nstate <= rdn5;
			elsif (pstate = rdn5) then
				nstate <= rdn6;
			elsif (pstate = rdn6) then
				nstate <= rdn7;
			elsif (pstate = rdn7) then
				nstate <= rdn8;
			elsif (pstate = rdn8) then
				nstate <= rdn9;
			elsif (pstate = rdn9) then
				nstate <= wtn;
			end if;
		end if;	
	end process;
	
	output_logic : process(pstate)
	begin
		if (pstate = wtn) then
			CNT_EN <= '0';
			postn <= "XXXX";
		elsif (pstate = rdn0) then
			CNT_EN <= '1';
			postn <= "0000";
		elsif (pstate = rdn1) then
			CNT_EN <= '1';
			postn <= "0001";
		elsif (pstate = rdn2) then
			CNT_EN <= '1';
			postn <= "0010";
		elsif (pstate = rdn3) then
			CNT_EN <= '1';
			postn <= "0011";
		elsif (pstate = rdn4) then
			CNT_EN <= '1';
			postn <= "0100";
		elsif (pstate = rdn5) then
			CNT_EN <= '1';
			postn <= "0101";
		elsif (pstate = rdn6) then
			CNT_EN <= '1';
			postn <= "0110";
		elsif (pstate = rdn7) then
			CNT_EN <= '1';
			postn <= "0111";
		elsif (pstate = rdn8) then
			CNT_EN <= '1';
			postn <= "1000";
		elsif (pstate = rdn9) then
			CNT_EN <= '1';
			postn <= "1001";
		end if;
	end process;
	
end behavioral;
