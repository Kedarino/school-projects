-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2024 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xlibra02 <login AT stud.fit.vutbr.cz>
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
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_INV  : out std_logic;                      -- pozadavek na aktivaci inverzniho zobrazeni (1)
   OUT_WE   : out std_logic;                      -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'

   -- stavove signaly
   READY    : out std_logic;                      -- hodnota 1 znamena, ze byl procesor inicializovan a zacina vykonavat program
   DONE     : out std_logic                       -- hodnota 1 znamena, ze procesor ukoncil vykonavani programu (narazil na instrukci halt)
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
    
architecture behavioral of cpu is   
    -- CNT 
    signal CNT : std_logic_vector(7 downto 0) := (others => '0');
    signal CNT_INC : std_logic := '0';
    signal CNT_DEC : std_logic := '0';
    
    signal CNT_ISZERO : std_logic;
    
    -- PTR: ukazatel do pameti dat
    signal PTR : std_logic_vector(12 downto 0)  := (others => '0');
    signal PTR_INC : std_logic := '0';
    signal PTR_DEC : std_logic := '0';
    signal PTR_RST : std_logic := '0';

    -- PC: 
    signal PC : std_logic_vector(12 downto 0)  := (others => '0');
    signal PC_INC : std_logic := '0';
    signal PC_DEC : std_logic := '0';

    -- MX
    signal MX1_SEL : std_logic := '1';  -- vyber adresy pro paměť (0: PTR, 1: PC)
    signal MX2_SEL : std_logic_vector(1 downto 0) := "11"; -- vyber dat pro zapis do pameti

    signal INIT_STOP : std_logic := '0';
    
    -- TMP
    signal TMP : std_logic_vector(7 downto 0) := (others => '0');

    -- Definice stavu stavoveho automatu
    type fsm_state is (
        idle,   
        default,            
        init, init_wait, init_read, init_wait1,              
        fetch, fetch_wait,              
        decode,            
        inc_ptr,            
        stabilize_ptr_inc,
        dec_ptr,            
        stabilize_ptr_dec,
        inc_data_read,     
        stabilize_inc_data_read,
        inc_data_write,     
        dec_data_read,     
        stabilize_dec_data_read,
        dec_data_write,
        dec_data_write_wait, 
        inc_data_write_wait,   
        output_data_read,   
        output_data_read_wait,
        output_data_check,   
        output_data_check1, 
        output_data_wait,
        input_data,        
        input_data_wait,   
        input_data_wait1,
        input_wait,
        loop_start,
        loop_start_check,
        loop_start_search,
        loop_read_wait, loop_read_wait1, loop_read_wait2, loop_read_wait3,
        loop_check_wait, loop_check_wait1, loop_check_wait2, loop_check_wait3,
        loop_end,
        loop_end_check,
        loop_end_search,
        halt,
        save_tmp, save_tmp_wait, 
        load_tmp, load_tmp_wait,              
        not_opcode          
    );
    signal CURR_STATE : fsm_state := default;
    signal NEXT_STATE : fsm_state;



begin
    -- CNT
    CNT_PROCESS : process (CLK, RESET)
    begin
        if (RESET = '1') then
            CNT <= (others => '0');
        
        elsif (rising_edge(CLK)) then
            if (CNT_INC = '1') then
                CNT <= CNT + 1;
            elsif (CNT_DEC = '1') then
                CNT <= CNT - 1;
            end if;
        end if;
    end process;

    CNT_ISZERO_PROCESS : process (CLK, CNT)
    begin 
        if (rising_edge(CLK)) then
            if( CNT = "00000000") then
                CNT_ISZERO <= '1';
            else
                CNT_ISZERO <= '0';
            end if;
        end if;
    end process;

    -- PTR
    PTR_PROCESS : process (CLK, RESET)
    begin 
        if (RESET = '1') then
            PTR <= (others => '0');
        
        elsif (rising_edge(CLK)) then  
            if (PTR_INC = '1') then
                PTR <= PTR + 1;
            elsif (PTR_DEC = '1') then
                PTR <= PTR - 1;
            elsif (PTR_RST = '1') then
                PTR <= (others => '0');
            end if;
        end if;
    end process;    
    
    -- PC 
    PC_PROCESS : process (CLK, RESET)
    begin
        if (RESET = '1') then
            PC <= (others => '0');
        
        elsif (rising_edge(CLK)) then
            if (PC_INC = '1') then
                PC <= PC + 1;
            elsif (PC_DEC = '1') then
                PC <= PC - 1;
            end if;
        end if;
    end process; 

    -- MX1
    MX1_PROCESS : process (PTR, PC, MX1_SEL)
    begin   
        if (MX1_SEL = '0') then
            DATA_ADDR <= PTR;
        elsif (MX1_SEL = '1') then
            DATA_ADDR <= PC;
        else
            null;
        end if;
    end process;
    
    -- MX2
    MX2_PROCESS : process (IN_DATA, DATA_RDATA, MX2_SEL)
    begin   
        if MX2_SEL = "00" then
            DATA_WDATA <= IN_DATA;
        elsif MX2_SEL = "01" then
            DATA_WDATA <= DATA_RDATA - 1;
        elsif MX2_SEL = "10" then 
            DATA_WDATA <= DATA_RDATA + 1;
        elsif MX2_SEL = "11" then
            DATA_WDATA <= TMP;
        else
            DATA_WDATA <= (others => '0');
        end if;
    end process;

    -- FSM
    FSM_CURRENT_STATE : process (CLK, RESET)
    begin   
        if RESET = '1' then
            CURR_STATE <= default;
        elsif (rising_edge(CLK)) then
            if(EN = '1') then
            CURR_STATE <= NEXT_STATE;
            end if;
        end if;
    end process;
    
    FSM_NEXT_STATE : process (CURR_STATE, EN, RESET, DATA_RDATA, PTR, CNT, IN_VLD, OUT_BUSY)
    begin

        PC_INC <= '0';
        PC_DEC <= '0';
        PTR_INC <= '0';
        PTR_DEC <= '0';
        

            case CURR_STATE is

                when default =>
                    MX2_SEL   <= "11"; 
                    DATA_EN   <= '0';
                    DATA_RDWR <= '1';
                    IN_REQ    <= '0';
                    OUT_WE    <= '0';
                    OUT_DATA  <= (others => '0');
                    PTR_RST   <= '0';
                    CNT_INC   <= '0';
                    CNT_DEC   <= '0';
                    MX1_SEL   <= '1';
                    OUT_INV   <= '0';
                    READY     <= '0';
                    DONE      <= '0';
                    INIT_STOP <= '0';
                    NEXT_STATE <= init;


                when idle =>
                    if (EN = '1') then
                       
                        NEXT_STATE <= fetch;
                    else
                        NEXT_STATE <= idle;
                    end if;

                when init =>
                    if EN = '1' then
                        

                        MX1_SEL <= '0';
                        DATA_EN <= '1';
                        DATA_RDWR <= '1';
                        NEXT_STATE <= init_read;  
                    else
                        NEXT_STATE <= init;
                    end if;

                when init_read =>
                    
                    if DATA_RDATA = x"40" then
                        
                        PTR_INC <= '1';
                        INIT_STOP <= '1';
                        NEXT_STATE <= init_wait;
                    else
                        
                        PTR_INC <= '1';
                        NEXT_STATE <= init_wait;
                    end if;

                when init_wait =>
                    if INIT_STOP = '1' then
                        READY <= '1';
                        NEXT_STATE <= init_wait1;
                    else
                        NEXT_STATE <= init_read;
                    end if;
                
                when init_wait1 =>
                    NEXT_STATE <= fetch;

                when fetch =>
                    MX1_SEL <= '1';
                    DATA_EN <= '1';
                    DATA_RDWR <= '1';
                    NEXT_STATE <= fetch_wait;
                
                when fetch_wait =>
                    NEXT_STATE <= decode;

                        
                when decode =>
                case DATA_RDATA is
                    when x"3E" =>   -- '>' - inkrementace PTR
                        NEXT_STATE <= inc_ptr;  
                    when x"3C" =>   -- '<' - dekrementace PTR
                        NEXT_STATE <= dec_ptr; 
                    when x"2B" =>   -- '+' - inkrementace dat na adrese PTR
                        MX1_SEL <= '0';
                        NEXT_STATE <= inc_data_read; 
                    when x"2D" =>   -- '-' - dekrementace dat na adrese PTR
                        MX1_SEL <= '0';
                        NEXT_STATE <= dec_data_read; 
                    when x"5B" =>   -- '[' - zacatek smycky
                        NEXT_STATE <= loop_start; 
                    when x"5D" =>   -- ']' - konec smycky
                        NEXT_STATE <= loop_end;   
                    when x"2E" =>   -- '.' - vystup dat
                        MX1_SEL <= '0';      
                        NEXT_STATE <= output_data_read; 
                    when x"2C" =>   -- ',' - vstup dat
                        NEXT_STATE <= input_data;
                    when x"24" =>   -- '$' - ulození do TMP
                        NEXT_STATE <= save_tmp;
                    when x"21" =>   -- '!' - nactení z TMP
                        NEXT_STATE <= load_tmp;
                    when x"40" =>   -- '@' - halt
                        NEXT_STATE <= halt; 
                    when others =>
                        NEXT_STATE <= not_opcode;   -- neznama instrukce
                end case;

                -- Implementace jednotlivych instrukci
                when inc_ptr =>
                    PC_INC <= '1';
                    PTR_INC <= '1';
                    NEXT_STATE <= fetch;

                
                when dec_ptr =>
                    PC_INC <= '1';
                    PTR_DEC <= '1';
                    NEXT_STATE <= fetch;

                when inc_data_read =>
                    DATA_EN <= '1';
                    DATA_RDWR <= '1';
                    NEXT_STATE <= stabilize_inc_data_read;

                when stabilize_inc_data_read =>
                    DATA_RDWR <= '0';
                    PC_INC <= '1';
                    NEXT_STATE <= inc_data_write;

                when inc_data_write =>
                    MX2_SEL <= "10";
                    NEXT_STATE <= inc_data_write_wait;
                
                when inc_data_write_wait =>
                    NEXT_STATE <= fetch;
                                    
                when dec_data_read =>
                    DATA_EN <= '1';
                    DATA_RDWR <= '1';
                    NEXT_STATE <= stabilize_dec_data_read;

                when stabilize_dec_data_read =>
                    MX2_SEL <= "01";
                    DATA_RDWR <= '0';
                    PC_INC <= '1';
                    NEXT_STATE <= dec_data_write;

                when dec_data_write =>
                    DATA_EN <= '1';
                    NEXT_STATE <= dec_data_write_wait;
                
                when dec_data_write_wait =>
                    NEXT_STATE <= fetch;
                
                when loop_start =>
                    MX1_SEL <= '0';
                    DATA_RDWR <= '1';
                    DATA_EN <= '1';
                    NEXT_STATE <= loop_read_wait;
                
                when loop_read_wait =>
                    NEXT_STATE <= loop_read_wait1;
                
                when loop_read_wait1 =>
                    NEXT_STATE <= loop_start_check;
                
                when loop_start_check =>
                    if DATA_RDATA = x"0" then
                        PC_INC <= '1';
                        MX1_SEL <= '1';
                        NEXT_STATE <= loop_check_wait;
                    else
                        MX1_SEL <= '1';
                        PC_INC <= '1';
                        NEXT_STATE <= fetch;
                    end if;
                
                when loop_check_wait =>
                        NEXT_STATE <= loop_check_wait1;
                
                when loop_check_wait1 =>
                        NEXT_STATE <= loop_start_search;

                when loop_start_search =>
                    if DATA_RDATA = x"5D" and CNT = x"0" then
                        PC_INC <= '1';
                        NEXT_STATE <= fetch;
                    
                    elsif DATA_RDATA = x"5B" then
                        CNT_INC <= '1';
                        PC_INC <= '1';
                        NEXT_STATE <= loop_check_wait;
                    
                    elsif DATA_RDATA = x"5D" and CNT /= x"0" then
                        CNT_DEC <= '1';
                        PC_INC <= '1';
                        NEXT_STATE <= loop_check_wait;
                    
                    else
                        PC_INC <= '1';
                        NEXT_STATE <= loop_check_wait;
                    end if;

                when loop_end =>
                        MX1_SEL <= '0';
                        DATA_RDWR <= '1';
                        DATA_EN <= '1';
                        NEXT_STATE <= loop_read_wait2;
                
                when loop_read_wait2 =>
                        NEXT_STATE <= loop_read_wait3;
                
                when loop_read_wait3 =>
                        NEXT_STATE <= loop_end_check;

                when loop_end_check =>
                        if DATA_RDATA = x"0" then
                            PC_INC <= '1';
                            MX1_SEL <= '1';
                            NEXT_STATE <= fetch;
                        else
                            PC_DEC <= '1';
                            MX1_SEL <= '1';
                            NEXT_STATE <= loop_check_wait2;
                        end if;
                when loop_check_wait2 =>
                        NEXT_STATE <= loop_check_wait3;
                
                when loop_check_wait3 =>
                        NEXT_STATE <= loop_end_search;
                
                when loop_end_search => 
                    if DATA_RDATA = x"5B" and CNT = x"0" then
                        PC_INC <= '1';
                        NEXT_STATE <= fetch;
                    
                    elsif DATA_RDATA = x"5D" then
                        PC_DEC <= '1';
                        CNT_INC <= '1';
                        NEXT_STATE <= loop_check_wait2;
                    
                    elsif DATA_RDATA = x"5B" and CNT /= x"0" then
                        PC_DEC <= '1';
                        CNT_DEC <= '1';
                        NEXT_STATE <= loop_check_wait2;
                    
                    else
                        PC_DEC <= '1';
                        NEXT_STATE <= loop_check_wait2;
                    end if;
                
                when output_data_read =>
                    DATA_RDWR <= '1';      
                    DATA_EN <= '1';          
                    NEXT_STATE <= output_data_read_wait;
                
                when output_data_read_wait =>
                    NEXT_STATE <= output_data_check;

                when output_data_check =>
                   
                    if OUT_BUSY = '0' then
                        
                        OUT_DATA <= DATA_RDATA;   
                        OUT_WE <= '1';            
                        
                        NEXT_STATE <= output_data_wait;
                    else
                        
                        NEXT_STATE <= output_data_check1;
                    end if;

                    when output_data_check1 =>
                    
                    if OUT_BUSY = '0' then
                        
                        OUT_DATA <= DATA_RDATA;   
                        OUT_WE <= '1';            
                        
                        NEXT_STATE <= output_data_wait;
                    else
                        
                        NEXT_STATE <= output_data_check;
                    end if;
                
                
                when output_data_wait =>
                        OUT_WE <= '0';
                        PC_INC <= '1';
                        NEXT_STATE <= fetch;

                when input_data =>
                    IN_REQ <= '1';
                    MX2_SEL <= "00";
                    DATA_EN <= '1';             
                    NEXT_STATE <= input_data_wait;

                when input_data_wait =>
                    if IN_VLD = '1' then
                        DATA_RDWR <= '0';      
                        MX1_SEL <= '0';        
                        PC_INC <= '1';         
                        IN_REQ <= '0';         
                        NEXT_STATE <= fetch;   
                    else
                        
                        NEXT_STATE <= input_data_wait1;
                    end if;

                    when input_data_wait1 =>
                    if IN_VLD = '1' then
                        DATA_RDWR <= '0';      
                        MX1_SEL <= '0';        
                        PC_INC <= '1';         
                        IN_REQ <= '0';         
                        NEXT_STATE <= fetch;   
                    else
                        
                        NEXT_STATE <= input_data_wait;
                    end if;

                    when input_wait =>
                        NEXT_STATE <= fetch;
                
                when save_tmp =>
                    MX1_SEL   <= '0';       
                    DATA_EN   <= '1';       
                    DATA_RDWR <= '1';       
                    NEXT_STATE <= save_tmp_wait;

                when save_tmp_wait =>
                    TMP <= DATA_RDATA;      
                    PC_INC <= '1';          
                    NEXT_STATE <= fetch;    

                
                when load_tmp =>
                    MX1_SEL   <= '0';       
                    DATA_EN   <= '1';      
                    DATA_RDWR <= '0';       
                    MX2_SEL   <= "11";     
                    NEXT_STATE <= load_tmp_wait;

                when load_tmp_wait =>
                    PC_INC <= '1';          
                    NEXT_STATE <= fetch;    

                when halt =>
                    DONE <= '1';
                    NEXT_STATE <= halt;
                
                when not_opcode =>
                    PC_INC <= '1';
                    NEXT_STATE <= fetch;
                    
                when others =>
                    PC_INC <= '1';
                    NEXT_STATE <= fetch;
            end case;
    end process;
end behavioral;