# Disease-transmission-model-in-C
This project is from the final project of the data structures course created by our group.

1. Why did we choose this topic?
We chose this topic not only because we wanted to focus on a real-world issue, but also because the threat of COVID-19 has left a deep impact on people’s minds, both in Taiwan and globally. Therefore, through this topic, we aimed to gain a deeper understanding and analysis of the issue, while applying the graph concepts we learned in class. The program for this project was initially based on the disease transmission model provided by ChatGPT. We primarily used the disease transmission model as the foundation, defining various parameters, transmission rates, and implementing different pandemic prevention policies. Additionally, we set the initial number of infected individuals to 77,744 to correspond with the population of the West Central Dist, Tainan City, simulating a more realistic scenario.

2. How to create Graph (code)?
    ![image](https://github.com/user-attachments/assets/b8979544-31ce-41ae-9c5f-3f1f82863293)

3. How to create node and edge (code) ?
   ![image](https://github.com/user-attachments/assets/4eb96795-aa44-4447-b512-938da0ad0e76)
   ![image](https://github.com/user-attachments/assets/4cfe5b8e-f25f-4ebe-82b3-7767f7c3393b)
4. SIR Disease Spread Modeling algorithm (code and explaination).
   ![image](https://github.com/user-attachments/assets/e1659e6c-99fa-4f63-8841-b514c5afd5e7)
   ![image](https://github.com/user-attachments/assets/c6fddd51-155b-4fec-a83b-094b814de178)

This function simulates a disease transmission model. In the for loop, it traces each node and checks whether the node is infected. If the node is infected, it continues to inspect the node's adjacency list to find each neighboring node (connected by an edge). Each node has a specific probability of becoming infected, and the infection probability varies depending on whether the node is in a closed environment (simulating a higher transmission rate indoors). Based on this, it determines whether any neighboring node gets infected.
  ![image](https://github.com/user-attachments/assets/8c749db1-b754-4f05-ae6c-4b49225f27b9)
This if statement determines that after a node is classified as infected, it has a certain probability of either dying or recovering.
  ![image](https://github.com/user-attachments/assets/2d5f70f5-7b82-43a7-b2ac-cf4089a493b0
If the node is not in the infected state but in the recovered state, the function calculates the reinfection rate. If it has been less than 200 days since recovery, there is a chance of reinfection.

The main function constructs the desired parameters and the graph model for the simulation.
![image](https://github.com/user-attachments/assets/3b541930-f0aa-415b-aec4-f395f3165c1a)
![image](https://github.com/user-attachments/assets/156b0364-0816-4cd6-b36f-64b454451441)

5. Implement public health policies and simulate the result by algorithm.
  (1) Output result without implementing any pandemic prevention policies (until INFECTED = 0):
    ![image](https://github.com/user-attachments/assets/da8c48ff-9feb-493b-9870-c3d58f0f810f)
   (2). Implementing Quarantine Policy:
      If an infection lasts for more than seven days, there is an 80% chance that the node's status        will change to "quarantined".
     ![image](https://github.com/user-attachments/assets/192e646e-accd-42ef-b177-11a9b025fd13)
     ![image](https://github.com/user-attachments/assets/a42ae82e-b145-45d8-95b9-da10697fc775)
     ![image](https://github.com/user-attachments/assets/84052997-f407-4fc6-aed4-1ea747059c8c)
     ![image](https://github.com/user-attachments/assets/cae742ad-aab1-4c27-86ab-034ad8caba74)

      Output result:
       ![image](https://github.com/user-attachments/assets/e966408a-6147-4df2-8a5c-9f914e32cdcd)
    (3). Implementing Vaccination Policy:
      In this policy, we introduced a new variable, vaccinationRate, and during the tracing of             nodes, each node in the SUSCEPTIBLE state has a certain probability (1%) of changing to the          VACCINATED state. This allows us to simulate the scenario where some nodes automatically             receive vaccinations during the disease transmission process, reducing their chances of              becoming infected and helping control the spread of the disease.
   ![image](https://github.com/user-attachments/assets/36e6c720-ebe4-4f1e-9ecc-4a430c49f893)
   ![image](https://github.com/user-attachments/assets/3aefa481-3251-4b77-bc04-4e4ac37201ec)
   ![image](https://github.com/user-attachments/assets/559f0d9c-c6bb-4819-895c-ed785fe21805)
   ![image](https://github.com/user-attachments/assets/10f0d2fa-9d8b-4409-85eb-c98a33ecc12a)
   ![image](https://github.com/user-attachments/assets/8f5c64fd-653d-4ff1-9802-d5307a079113)
      Output result:
   ![image](https://github.com/user-attachments/assets/b86028bf-c8ec-4487-bc9d-f7f01a403d0a)

    (4). Implementing Quarantine & Vaccination Policy:

As the name suggests, this policy combines the features of both quarantine and vaccination, allowing nodes to exist in the following states: SUSCEPTIBLE, INFECTED, RECOVERED, DEAD, ISOLATED, and VACCINATED.

During the initialization phase, the program randomly selects a portion of the nodes and sets their state to VACCINATED. Vaccinated nodes have a reduced infection rate during the disease transmission process.
If a node becomes infected, its state is set to INFECTED, and the number of days infected is tracked.
After a node has been infected for more than 7 days, there is an 80% chance that it will transition to the ISOLATED state. While isolated, the node no longer spreads the infection but still has a chance to either recover or die.
For SUSCEPTIBLE nodes, there is a 1% chance each day that they will transition to the VACCINATED state.
    ![image](https://github.com/user-attachments/assets/9c033922-59c9-4a09-9c82-58b085ffc209)
    ![image](https://github.com/user-attachments/assets/9a0df6e3-73f8-4458-9f5b-d66a2cf46630)
    ![image](https://github.com/user-attachments/assets/e2e389b9-9bdd-46ce-bb1d-523aa26c4120)
    ![image](https://github.com/user-attachments/assets/22d9d4c2-79fa-4e3d-bdab-e89c6b84c34d)
    ![image](https://github.com/user-attachments/assets/21fa4d8a-89dd-4d5b-83c7-1f39a1b2507d)
    ![image](https://github.com/user-attachments/assets/449d061b-0269-422d-b0cc-4238d92b2ae8)

    Output result:
     ![image](https://github.com/user-attachments/assets/076654cd-7338-46d8-a9a2-bab49f606e29)

       




