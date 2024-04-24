# BOURDEAU & COLLIN

import subprocess
import time

# Chemin vers le fichier
chemin_fichier = "C:/Users/samco/OneDrive - IPSA/AERO4/TOULOUSE/Modélisation_Des_Systemes_Information_Complexe/TP_C++/Final_task/print_working.exe"

nombre_executions = 100
temps_executions = []

for i in range(nombre_executions):
    # Enregistre le temps de début
    debut = time.time()
    
    # Exécute le fichier. Assurez-vous que le chemin et les permissions sont corrects.
    subprocess.run(chemin_fichier, shell=True)
    
    # Enregistre le temps de fin et calcule le temps d'exécution
    fin = time.time()
    temps_executions.append(fin - debut)
    
    print(f"Execution {i+1}: {fin - debut} secondes")

# Calcule le temps d'exécution moyen
WCET = max(temps_executions)
print(f"Pire temps d'execution : {WCET} secondes")
