-- Citac s volitelnou frekvenci test bench
-- IVH projekt - ukol2
-- autor: Dmytro Sadovskyi (xsadov06)

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
USE ieee.numeric_std.ALL;

 
ENTITY counter_tb IS
END counter_tb;
 
ARCHITECTURE behavior OF counter_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT counter
	 Generic (
			CLK_FREQ : positive := 1000000;
			OUT_FREQ : positive := 100000);	
    PORT(
         CLK : IN  std_logic;
         RESET : IN  std_logic;
         EN : OUT  std_logic
        );
    END COMPONENT;
   --Inputs
   signal CLK : std_logic := '0';
   signal RESET : std_logic := '0';

 	--Outputs
   signal EN : std_logic;

   -- Clock period definitions
   constant CLK_period : time := 1 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
	tb : counter
		GENERIC MAP(CLK_FREQ  => 1000000,
						 OUT_FREQ  => 10000)
		PORT MAP(CLK => CLK,
					RESET => RESET,
					EN => EN );
	-- vas ukol !!!!!!!!! vcetne nastaveni generickych parametru
   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin	
			RESET <= '1';
		wait until EN = '0';
			RESET <= '0';
		wait until EN = '1';
		wait until CLK'event and CLK = '1';
			RESET <= '1';
		wait;
   end process;
	
	assert_proc: process
	
	begin
	wait for 100 ns;
	assert (EN = '1')	report "EN is not set to '1'";
	wait;
	end process; 


END;
