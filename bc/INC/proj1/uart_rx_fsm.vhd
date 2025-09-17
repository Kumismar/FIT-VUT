-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Name Surname (xlogin00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



entity UART_RX_FSM is
    port(
        CLK : in std_logic;
        RST : in std_logic;
        DIN : in std_logic;
        CNT : in std_logic_vector(3 downto 0);
        CNT_BIT : in std_logic_vector(2 downto 0);

        CNT_R: out std_logic;
        SHIFT: out std_logic;
        DOUT_VLD: out std_logic;
        CNT_BIT_INC: out std_logic
    );
end entity;

architecture behavioral of UART_RX_FSM is
    type state is (IDLE, WAIT_MIDBIT, WAIT_READ, READ_BIT, WAIT_STOP);
    signal pstate, nstate: state;
begin

    pstatereg: process(RST, CLK)
    begin
        if (RST='1') then
            pstate <= IDLE;
        elsif (CLK'event) and (CLK='1') then
            pstate <= nstate;
        end if;
    end process;

    nstate_logic: process(pstate, DIN, CNT, CNT_BIT)
    begin
        -- default values
        nstate <= IDLE;
        CNT_R <= '0';
        CNT_BIT_INC <= '0';
        DOUT_VLD <= '0';
        SHIFT <= '0';

        case pstate is
            when IDLE =>
                nstate <= IDLE;

                if (DIN = '0') then
                    nstate <= WAIT_MIDBIT;
                    CNT_R <= '1';
                end if;
        
            when WAIT_MIDBIT =>
                nstate <= WAIT_MIDBIT;
                if (CNT = "0111") then
                    nstate <= WAIT_READ;
                    CNT_R <= '1';
                end if;

            when WAIT_READ =>
                nstate <= WAIT_READ;
                if (CNT = "1111") then
                    nstate <= READ_BIT;
                end if;
                    
            when READ_BIT =>
                nstate <= WAIT_READ;
                SHIFT <= '1';
                CNT_BIT_INC <= '1';
                if (CNT_BIT = "111") then
                    nstate <= WAIT_STOP;
                end if;
            
            when WAIT_STOP =>
                nstate <= WAIT_STOP;
                if (CNT = "1111" and DIN = '1') then
                    nstate <= IDLE;
                    DOUT_VLD <= '1';
                end if;
            
            when others => null;
        end case;

    end process;

end architecture;
