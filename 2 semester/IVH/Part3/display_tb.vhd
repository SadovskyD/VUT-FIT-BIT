LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY display_tb IS
END display_tb;
 
ARCHITECTURE behavior OF display_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT display
    PORT(
         data : IN  std_logic_vector(127 downto 0);
         reset : IN  std_logic;
         clk : IN  std_logic;
         smclk : IN  std_logic;
         A : OUT  std_logic_vector(3 downto 0);
         R : OUT  std_logic_vector(7 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal data : std_logic_vector(127 downto 0) := (others => '0');
   signal reset : std_logic := '0';
   signal clk : std_logic := '0';
   signal smclk : std_logic := '0';

 	--Outputs
   signal A : std_logic_vector(3 downto 0);
   signal R : std_logic_vector(7 downto 0);

   -- Clock period definitions
   constant clk_period : time := 40 ns;
   constant smclk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: display PORT MAP (
          data => data,
          reset => reset,
          clk => clk,
          smclk => smclk,
          A => A,
          R => R
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 
   smclk_process :process
   begin
		smclk <= '0';
		wait for smclk_period/2;
		smclk <= '1';
		wait for smclk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin
		data <=
		"0000000000000000" &
		"0000000000000000" &
		"0000110000110000" &
		"0000101001000000" &
		"0000101000100000" &
		"0000101000010000" &
		"0000110001100000" &
		"0000000000000000";
		wait for 821ns;
		assert (A = "0000" and R = "00000000") report "sloupec 0 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0001" and R = "00000000") report "sloupec 1 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0010" and R = "00000000") report "sloupec 2 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0011" and R = "00000000") report "sloupec 3 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0100" and R = "01111100") report "sloupec 4 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0101" and R = "01000100") report "sloupec 5 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0110" and R = "00111000") report "sloupec 6 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0111" and R = "00000000") report "sloupec 7 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1000" and R = "00000000") report "sloupec 8 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1001" and R = "01001000") report "sloupec 9 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1010" and R = "01010100") report "sloupec 10 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1011" and R = "00100100") report "sloupec 11 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1100" and R = "00000000") report "sloupec 12 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1101" and R = "00000000") report "sloupec 13 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1110" and R = "00000000") report "sloupec 14 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1111" and R = "00000000") report "sloupec 15 neodpovídá oèekavanemu";
		data <=
		"1000001001000001" &
		"1000000110000001" &
		"1000000110000001" &
		"1000001001000001" &
		"1000010000100001" &
		"1000100000010001" &
		"1001000000001001" &
		"1010000000000101";
	   wait for 840ns;
		assert (A = "0000" and R = "11111111") report "sloupec 0 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0001" and R = "00000000") report "sloupec 1 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0010" and R = "10000000") report "sloupec 2 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0011" and R = "01000000") report "sloupec 3 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0100" and R = "00100000") report "sloupec 4 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0101" and R = "00010000") report "sloupec 5 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0110" and R = "00001001") report "sloupec 6 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "0111" and R = "00000110") report "sloupec 7 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1000" and R = "00000110") report "sloupec 8 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1001" and R = "00001001") report "sloupec 9 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1010" and R = "00010000") report "sloupec 10 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1011" and R = "00100000") report "sloupec 11 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1100" and R = "01000000") report "sloupec 12 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1101" and R = "10000000") report "sloupec 13 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1110" and R = "00000000") report "sloupec 14 neodpovídá oèekavanemu";
		wait for 840ns;
		assert (A = "1111" and R = "11111111") report "sloupec 15 neodpovídá oèekavanemu";
	   wait;
		end process;

END;
