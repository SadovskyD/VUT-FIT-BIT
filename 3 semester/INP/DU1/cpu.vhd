-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2021 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Dmytro Sadovskyi
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WREN  : out std_logic;                    -- cteni z pameti (DATA_WREN='0') / zapis do pameti (DATA_WREN='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WREN musi byt '0'
   OUT_WREN : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	-- PTR
	signal ptr_reg : std_logic_vector(9 downto 0);
	signal ptr_inc : std_logic;
	signal ptr_dec : std_logic;
	
   --PC
	signal pc_reg : std_logic_vector(11 downto 0);
	signal pc_inc : std_logic;
	signal pc_dec : std_logic;

	-- B_CNT
	signal cnt_reg : std_logic_vector(7 downto 0);
	signal cnt_inc : std_logic;
	signal cnt_dec : std_logic;
	signal cnt_reset : std_logic;

	-- MUX
	signal mx_sel : std_logic_vector(1 downto 0) := "00";

	--FSM
	type fsm_state is (
		st_fetch, st_decode, st_ignore, st_inc_ptr, st_dec_ptr, st_inc_0, st_inc_1, st_inc_2,
		st_dec_0, st_dec_1, st_dec_2,	st_while_0, st_while_1, st_while_2, st_while_3,
		st_end_0, st_end_1, st_end_2, st_end_3, st_end_4, st_end_5,
		st_output_0, st_output_1, st_read_0, st_read_1, st_break_0, st_break_1, st_break_2,
		st_null
	);
	signal fsm_pstate : fsm_state;
	signal fsm_nstate : fsm_state;

begin
	
	ptr_proc: process (CLK, RESET)
	begin
		if RESET = '1' then
			ptr_reg <= "0000000000";
		elsif rising_edge(CLK) then
			if ptr_dec = '1' then
				ptr_reg <= ptr_reg - 1;
			elsif ptr_inc = '1' then
				ptr_reg <= ptr_reg + 1;
			end if;
		end if;
	end process;
	
	pc_proc: process (CLK, RESET)
	begin
		if RESET = '1' then
			pc_reg <= "000000000000";
		elsif rising_edge(CLK) then
			if pc_inc = '1' then
				pc_reg <= pc_reg + 1;
			elsif pc_dec = '1' then
				pc_reg <= pc_reg - 1;
			end if;
		end if;
	end process;

	bracket_cnt_proc: process (CLK, RESET, cnt_reset)
	begin
		if RESET = '1' then
			cnt_reg <= X"00";
		elsif rising_edge(CLK) then
			if cnt_dec = '1' then
				cnt_reg <= cnt_reg - 1;
			elsif cnt_inc = '1' then
				cnt_reg <= cnt_reg + 1;
			elsif cnt_reset = '1' then
				cnt_reg <= "00000001";
			end if;
		end if;
	end process;

	mux_proc: process (CLK, mx_sel)
	begin
		if rising_edge(CLK) then
			if (mx_sel = "00") then
					DATA_WDATA <= IN_DATA;
			elsif (mx_sel = "01") then
					DATA_WDATA <= DATA_RDATA + 1;
			elsif (mx_sel = "10") then
					DATA_WDATA <= DATA_RDATA - 1;
			end if;
		end if;
	end process;

	fsm_ps_proc: process (CLK, RESET, EN)
	begin
		if RESET = '1' then
			fsm_pstate <= st_fetch;
		elsif rising_edge(CLK) then
			if EN = '1' then
				fsm_pstate <= fsm_nstate;
			end if;
		end if;
	end process;

	fsm_ns_proc: process (CLK, CODE_DATA, DATA_RDATA, IN_VLD, cnt_reg, fsm_pstate)
	begin
		DATA_EN <= '0';
		DATA_WREN <= '0';
		OUT_WREN <= '0';
		IN_REQ <= '0';
		CODE_EN <= '0';
		pc_inc <= '0';
		pc_dec <= '0';
		ptr_inc <= '0';
		ptr_dec <= '0';
		cnt_inc <= '0';
		cnt_dec <= '0';
		mx_sel <= "00";
		cnt_reset <= '0';

		case fsm_pstate is

			when st_fetch =>
				CODE_EN <= '1';
				fsm_nstate <= st_decode;
			
			when st_decode =>
				case CODE_DATA is
					when X"3E" =>
						fsm_nstate <= st_inc_ptr;
					when X"3C" =>
						fsm_nstate <= st_dec_ptr;
					when X"2B" =>
						fsm_nstate <= st_inc_0;
					when X"2D" =>
						fsm_nstate <= st_dec_0;
					when X"5B" =>
						fsm_nstate <= st_while_0;
					when X"5D" =>
						fsm_nstate <= st_end_0;
					when X"2E" =>
						fsm_nstate <= st_output_0;
					when X"2C" =>
						fsm_nstate <= st_read_0;
					when X"7E" =>
						fsm_nstate <= st_break_0;
					when X"00" =>
						fsm_nstate <= st_null;
					when others =>
						fsm_nstate <= st_ignore;
				end case;

			when st_inc_0 =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				fsm_nstate <= st_inc_1;

			when st_inc_1 =>
				mx_sel <= "01";
				fsm_nstate <= st_inc_2;

			when st_inc_2 =>
				DATA_EN <= '1';
				DATA_WREN <= '1';
				pc_inc <= '1';
				fsm_nstate <= st_fetch;

			when st_dec_0 =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				fsm_nstate <= st_dec_1;

			when st_dec_1 =>
				mx_sel <= "10";
				fsm_nstate <= st_dec_2;

			when st_dec_2 =>
				DATA_EN <= '1';
				DATA_WREN <= '1';
				pc_inc <= '1';
				fsm_nstate <= st_fetch;

			when st_inc_ptr =>
				ptr_inc <= '1';
				pc_inc <= '1';
				fsm_nstate <= st_fetch;

			when st_dec_ptr =>
				ptr_dec <= '1';
				pc_inc <= '1';
				fsm_nstate <= st_fetch;

			when st_while_0 =>
				pc_inc <= '1';
				DATA_EN <= '1';
				DATA_WREN <= '0';
				fsm_nstate <= st_while_1;

			when st_while_1 =>
				if DATA_RDATA = X"00" then
					cnt_reset <= '1';
					fsm_nstate <= st_while_2;
				else
					fsm_nstate <= st_fetch;
				end if;

			when st_while_2 =>
				if cnt_reg = X"00" then
					fsm_nstate <= st_fetch;
				else
						CODE_EN <= '1';
						fsm_nstate <= st_while_3;
				end if;

			when st_while_3 =>
				if CODE_DATA = X"5B" then
					cnt_inc <= '1';
				elsif CODE_DATA = X"5D" then
					cnt_dec <= '1';
				end if;
				pc_inc <= '1';
				fsm_nstate <= st_while_2;

			when st_end_0 =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				fsm_nstate <= st_end_1;

			when st_end_1 =>
				if DATA_RDATA = X"00" then
					pc_inc <= '1';
					fsm_nstate <= st_fetch;
				else
					fsm_nstate <= st_end_2;
				end if;

			when st_end_2 =>
				cnt_reset <= '1';
				pc_dec <='1';
				fsm_nstate <= st_end_3;

			when st_end_3 =>
				if cnt_reg = X"00" then
					fsm_nstate <= st_fetch;
				else
					CODE_EN <= '1';
					fsm_nstate <= st_end_4;
				end if;

			when st_end_4 =>
				if CODE_DATA = X"5D" then
					cnt_inc <= '1';
				elsif CODE_DATA = X"5B" then
					cnt_dec <= '1';
				end if;
				fsm_nstate <= st_end_5;
			
			when st_end_5 =>
				if cnt_reg = X"00" then
					pc_inc <= '1';
				else
					pc_dec <= '1';
				end if;
				fsm_nstate <= st_end_3;

			when st_output_0 =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				fsm_nstate <= st_output_1;

			when st_output_1 =>
				if OUT_BUSY = '1' then
					DATA_EN <= '1';
					DATA_WREN <= '0';
					fsm_nstate <= st_output_1;
				else
					OUT_WREN <= '1';
					pc_inc <= '1';
					fsm_nstate <= st_fetch;
				end if;

			when st_read_0 =>
				mx_sel <= "00";
				IN_REQ <= '1';
				fsm_nstate <= st_read_1;

			when st_read_1 =>
				if IN_VLD /= '1' then
					mx_sel <= "00";
					IN_REQ <= '1';
					fsm_nstate <= st_read_1;
				else
					DATA_EN <= '1';
					DATA_WREN <= '1';
					pc_inc <= '1';
					fsm_nstate <= st_fetch;
				end if;

			when st_break_0 =>
				cnt_inc <= '1';
				pc_inc <= '1';
				fsm_nstate <= st_break_1;

			when st_break_1 =>
				CODE_EN <= '1';
				fsm_nstate <= st_break_2;
			
			when st_break_2 =>
				if cnt_reg = X"00" then
					fsm_nstate <= st_fetch;
				else
					if CODE_DATA = X"5B" then
						cnt_inc <= '1';
					elsif CODE_DATA = X"5D" then
						cnt_dec <= '1';
					end if;
					pc_inc <= '1';
					fsm_nstate <= st_break_1;
				end if;

			when st_null =>
				fsm_nstate <= st_null;

			when st_ignore =>
				pc_inc <= '1';
				fsm_nstate <= st_fetch;

			when others =>
				null;

		end case;
	end process;

	CODE_ADDR <= pc_reg;
	DATA_ADDR <= ptr_reg;
	OUT_DATA <= DATA_RDATA;
	
end behavioral;