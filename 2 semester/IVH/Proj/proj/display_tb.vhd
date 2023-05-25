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
         reset : IN  std_logic;
         clk : IN  std_logic;
         smclk : IN  std_logic;
         data : IN  std_logic_vector(127 downto 0);
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
	   wait;
		end process;

END;
