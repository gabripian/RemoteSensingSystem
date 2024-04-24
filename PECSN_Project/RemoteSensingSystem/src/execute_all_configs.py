import math
import time
import subprocess

def run_cmd(command):
    try:
        # Run the command and capture the output
        result = subprocess.run(command, shell=True, text=True, capture_output=True)
        # Print any errors
        if result.stderr:
            print("Error:")
            print(result.stderr)

    except Exception as e:
        print(f"An error occurred: {e}")

# List of commands to execute
commands_to_run = [
    "General",
    "Degenerancy-Test-Firm-Sensing-Nodes-Access-Point-Deterministic","Degenerancy-Test-No-Sensing-Nodes","Degenerancy-Test-No-Access-Points", #3
    "Degenerancy-Test-Psucc-1","Degenerancy-Test-Psucc-0", #5
    "Degenerancy-Test-D-0","Degenerancy-Test-D-max",#7
    
    "Consistency-Test-T-1","Consistency-Test-T-4",#9

    "Continuity-Test-Psucc-07","Continuity-Test-Psucc-065","Continuity-Test-Psucc-075",  #12
    "Continuity-Test-D-50","Continuity-Test-D-40","Continuity-Test-D-60", #15
    
    "The-2kr",
    "Avg-Analysis"
]

how_many_config = len(commands_to_run)
counter = 0

config_to_run = "General"

command_structure = "RemoteSensingSystem.exe -m -u Cmdenv -c {} -n ../simulations;. -s --scalar-recording=true --cmdenv-express-mode=true ../simulations/omnetpp.ini"

# Execute each command in the list
for command in commands_to_run:
    counter = counter + 1
    config_to_run = command
    
    command_to_execute = command_structure.format(config_to_run)
    print(command_to_execute)  # Separator between command outputs

    run_cmd(command_to_execute)

    print(math.trunc((counter/how_many_config)*100),'%')
    
    if ((counter % 3 == 0) and (counter > 15)) : #
        print("pause of 10 minutes...") 
        time.sleep(600)
        
   
print("Simulation campaign done!")