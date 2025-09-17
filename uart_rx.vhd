-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Ondrej Koumar (xkouma02)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is
    signal CNT: std_logic_vector(3 downto 0) := (others => '0');
    signal CNT_R: std_logic := '0';
    signal CNT_BIT: std_logic_vector(2 downto 0) := (others => '0');
    signal CNT_BIT_INC: std_logic := '0';
    signal SHIFT: std_logic := '0';
    signal SHREG_OUT: std_logic_vector(7 downto 0) := (others => '0');
begin

    -- Instance of RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        DOUT_VLD => DOUT_VLD,
        CNT => CNT,
        CNT_R => CNT_R,
        CNT_BIT => CNT_BIT,
        CNT_BIT_INC => CNT_BIT_INC,
        SHIFT => SHIFT
    );


    shift_reg: process(CLK)
    begin
        if (CLK'event) and (CLK='1') then
            if (SHIFT='1') then
                DOUT <= DIN & SHREG_OUT(7 downto 1);
                SHREG_OUT <= DIN & SHREG_OUT(7 downto 1);
            end if;
        end if;
    end process;

    counter: process(CLK, CNT_R)
    begin
        if (CLK'event) and (CLK='1') then
            if (CNT_R='1') then
                CNT <= (others => '0');
            else
                CNT <= CNT + 1;
            end if;
        end if;
    end process;

    counter_bit: process(CLK, CNT_BIT_INC)
    begin
        if (CLK'event) and (CLK = '1') then
            if (CNT_BIT_INC = '1') then
                CNT_BIT <= CNT_BIT + 1;
            end if;
        end if;
    end process;

end architecture;
