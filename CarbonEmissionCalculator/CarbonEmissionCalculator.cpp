/* This program performs the carbon footprint calculation for a household
*Authors: Khadiza Akter, Meng Huan Lee, Alonso Ibarra 
*Class: CIS-5-48643 
*Date: 12/09/21 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;

// This function reads the CSV file. The CSV file contains the zip code, state code, and electricity emission factor for different zip codes
vector<vector<string>> ReadCSVFile()
{

	vector<vector<string>> CSVEmissioncontent;
	vector<string> row;
	string line, word;
	fstream file("./EmissionRate.csv", ios::in); // The CSV file name is EmissionRate.csv
	if (file.is_open())
	{
		while (getline(file, line))
		{
			row.clear();

			stringstream str(line);

			while (getline(str, word, ','))
				row.push_back(word);
			CSVEmissioncontent.push_back(row);
		}
	}
	else
		cout << "Could not read the EmissionRate.csv file \n";

	return CSVEmissioncontent;
}
/*This function search the user zip code from the CSV file contents. The vector of CSV file is very large over 41000 elements, therefore, 
* it is passed as a reference and here the vector will not modify just search over it so it is a const reference. If the zip code is found then return true, otherwise it will
* return false.  Also, the function will set the value for state code and electricity emission factor as  reference parameters that
* can be accessed it from main function or from  where it call.
*/

bool SearchZipCode(const vector<vector<string>>& CSVFileEmissionContent, string zipCode, string& state, double& electricityEmissionFactor)
{
	bool isFound = false;
	const int ZIP_CODE_COL = 0, STATE_COL = 1, EMISSION_FACT_COL = 2; // column index 0 for zip code, column index 1 for state code, column index 2 for emission factor

	for (int i = 0; i < CSVFileEmissionContent.size(); i++)
	{
		if (CSVFileEmissionContent[i][ZIP_CODE_COL] == zipCode)
		{

			cout << "Zip code: " << CSVFileEmissionContent[i][ZIP_CODE_COL] << " ";
			state = CSVFileEmissionContent[i][STATE_COL];
			cout << "State: " << state << " ";
			electricityEmissionFactor = stod(CSVFileEmissionContent[i][EMISSION_FACT_COL]);
			cout << "Emission factor: " << electricityEmissionFactor << endl;
			isFound = true;
			break;
		}
	}
	return isFound;
}
/*This function calculates the vehicle emission, and how much emission can be saved from the vechicle. The input parameters of this
*function are total driven mileages per year, mileages per gallon, emission factor of passenger vehicle, and Non-CO2 vehicle emission
* ratio. The vehicle emission is calculated by the following formula:
* Vehicle Emission = (total driven mileages per year/mileages per gallon) * emission factor of passenger vehicle * Non-CO2 vehicle emission ratio.
* All calculated equations were extracted from the EPA excel sheet. The function returns the vehicle emission per year.
*/
double CalculateVehicleEmission(double drivenMilesPerYear, double mileagesPerGallon, const double EF_PassengerVeh, const double NonCO2VehEmissionRatio)
{
	double vehicleEmission = (drivenMilesPerYear / mileagesPerGallon) * EF_PassengerVeh * NonCO2VehEmissionRatio;
	return vehicleEmission;
}
/*This function calculates the household energy emission, the energy was calculated the monthly bills of various energy consumption.
* The input parameters of the function are monthly bill, cost of fuel, emission factor, and total month of a year.
*The energy emission is calculated by the following formula:
* Energy emission = (monthly bill / cost of fuel) * emission factor * total month of a year.
* The function returns the calculated energy emission equivalence to CO2 (lbs) per year.
*/
double CalculatedEnergyEmission(double monthlyBill, const double costOfFuel, const double emissionFactor, const int totalMonth)
{
	double calculatedEmission = (monthlyBill / costOfFuel) * emissionFactor * totalMonth;
	return calculatedEmission;
}
/*This function calculated the waste emission calculation. The waste emission was calculated from the number of household persons.
* The input parameters of the function are number of persons of a household and average waste emission.
* The calculated waste emission was calculated from the following equation:
* Waste Emission = number of persons * average waste emission.
* The function returns the calculated waste emission equivalence to CO2 (lbs) per year.
*/
double CalculatedWasteEmission(int numberOfPerson, const double averageWasteEmission)
{
	double calculatedWasteEmission = numberOfPerson * averageWasteEmission;
	return calculatedWasteEmission;
}
/*This function calculated the saving emission from household energy resources. The input parameters of the function are plan for yearly
* energy saving and electric emission factor for the specific zip code. The saving energy is calculated from the following equation:
* Emission Saving = yearly energy saving * electric emission factor.
* The function returns the calculated energy saving equivalence to CO2 (lbs) per year.
*/
double EnergyEmissionSaving(double yearlyEnergySaving, const double emissionFactor)
{
	double calculatedEnergyEmissionSaving = yearlyEnergySaving * emissionFactor;
	return calculatedEnergyEmissionSaving;
}

int CheckValidInput(int& reduceActionOrNot, string message)
{
	cout << message;
	cin.clear();
	cin.ignore(256, '\n');  //ignore the character \n
	cin >> reduceActionOrNot;
	return 0;
}

int CheckValidInput(double& reduceActionOrNot, string message)
{
	cout << message;
	cin.clear();
	cin.ignore(256, '\n');  //ignore the character \n
	cin >> reduceActionOrNot;
	return 0;
}

/*This function writes the summary report of carbon footprint in a text file for a household. The function writes the summary a text file
* that name is CarbonFootPrintReport.txt. The function inputs parameters are helped to prepare the summary report. The inputs parameters
* are total family member, state code, zip code, total household energy emission, total vehicle emission, total waste emission,
* emission saving from the household energy, energy saving from vehicle, energy saving from waste, total emission after savings,
* how much emission a tree can absorb per year.
 */
int WriteSummaryReport(int totalFamilyMember, string state, string zipCode, double totalHouseholdEmissionBeforeSaving, double totalVehicleEmissionBeforeSaving, double wasteEmissionAfterRecyle, double totalHouseholdEmissionSaving, double totalVehicleEmissionSaving, double totalWasteEmissionSaving, double totalEmissionAfterSaving, int TREE_ABSORB_CO2)
{
	ofstream outFS;
	outFS.open("CarbonFootprint.txt");
	if (!outFS.is_open()) {
		cout << "Could not open file CarbonFootprint.txt." << endl;
	}
	
	outFS << "----Summary of Your Carbon Footprint----Annual CO2 Emissions (lbs.)----" << "\n\n";

	outFS << "Total members: " << totalFamilyMember << " State: " << state << " Zip-code: " << zipCode << "\n";
	outFS << "Carbon Emission (CO2) is from the Household Energies: " << totalHouseholdEmissionBeforeSaving << " (lbs)\n";
	outFS << "Carbon Emission (CO2) is from the Household Vehicles: " << totalVehicleEmissionBeforeSaving << " (lbs)\n";
	outFS << "Carbon Emission (CO2) is from the Household Wastes: " << wasteEmissionAfterRecyle << " (lbs)\n";
	outFS << "Current total Carbon Emission (CO2) is from the Household: " << (totalHouseholdEmissionBeforeSaving + totalVehicleEmissionBeforeSaving + wasteEmissionAfterRecyle) << " (lbs)\n";
	outFS << "Number of trees is required to absorb the total emission: " << round((totalHouseholdEmissionBeforeSaving + totalVehicleEmissionBeforeSaving + wasteEmissionAfterRecyle) / TREE_ABSORB_CO2) << "\n\n";

	outFS << "Carbon Emission (CO2) is Savings after Planning:" << endl;
	outFS << "Carbon Emission (CO2) is Savings from the Household Energies: " << totalHouseholdEmissionSaving << " (lbs)\n";
	outFS << "Carbon Emission (CO2) is Savings from the Household Vehicles: " << totalVehicleEmissionSaving << " (lbs)\n";
	outFS << "Carbon Emission (CO2) is Savings from the Household Wastes: " << totalWasteEmissionSaving << " (lbs)\n";
	outFS << "Total Carbon Emission (CO2) is from the Household: " << (totalHouseholdEmissionSaving + totalVehicleEmissionSaving + totalWasteEmissionSaving) << " (lbs)\n";
	outFS << "Total Savings are Equivalence to the number of trees: " << round((totalHouseholdEmissionSaving + totalVehicleEmissionSaving + totalWasteEmissionSaving) / TREE_ABSORB_CO2) << "\n\n";
	outFS << "New Total Carbon Emission (CO2) is after taking the saving actions: " << totalEmissionAfterSaving << " (lbs)\n";
	outFS << "Number of trees is required to absorb the new total emission: " << round((totalEmissionAfterSaving) / TREE_ABSORB_CO2) << "\n\n";

	outFS.close();
	
	return 0;
}

int main()
{
	/*
	* Different consts are declared here which are the different standard values of emission calculated related factors. The constant
	* values are collected from the EPA's excel sheet.
	*/
	const double EMISSION_FACTOR_PASSENGER_VEHICLE = 19.6;
	const double EMISSION_FACTOR_GAS = 119.58;
	const double NON_CO2_VEHICLE_EMISSION_RATIO = 1.01;
	const double GAS_COST_THOUSAND_CUBIC_FEET = 10.68;
	const int TOTAL_MONTH_OF_YEAR = 12;
	const double COST_PER_KWH = 0.1188;
	const double FUEL_OIL_COST = 4.02;
	const double EMISSION_FACTOR_FUEL_OIL = 22.61;
	const double PROPANE_COST = 2.47;
	const double EMISSION_FACTOR_PROPANE = 12.43;
	const double AVERAGE_WASTE_EMISSION_PER_PERSON = 692;
	const double METAL_RECYCLE_AVOID_EMISSION = 89.38;
	const double MAGAZINE_RECYCLE_AVOID_EMISSION = 27.46;
	const double PLASTIC_RECYCLE_AVOID_EMISSION = 35.56;
	const double GLASS_RECYCLE_AVOID_EMISSION = 25.39;
	const double NEWSPAPER_RECYCLE_AVOID_EMISSION = 113.14;
	const double COMPUTER_SLEEP_SAVING = 107.1;
	const double LAMP_KWH_SAVING = 33.0;
	const int DRYER_ENERGY = 769;
	const int FRIDGE_KWH_SAVING = 322;
	const int TREE_ABSORB_CO2 = 48;
	int totalFamilyMember = 0, recyleOrNot = 0, reduceActionOrNot = 0, numberofBulbs = 0;
	int numberOfVehicle = 0;
	double drivenMilesPerYear = 0.0, milagesPerGallon = 0.0, totalVehicleEmissionBeforeSaving = 0.0, totalVehicleEmissionAfterSaving = 0.0, calculatedVehicleEmission = 0.0, totalVehicleEmissionSaving = 0.0;
	double monthlyGasBill = 0.0, monthlyElectricityBill = 0.0, monthlyFuelOilBill = 0.0, monthlyPropaneBill = 0.0;
	double gasEmission = 0.0, electricityEmission = 0.0, fuelOilEmission = 0.0, propaneEmission = 0.0, totalHouseholdEmissionBeforeSaving = 0.0, totalHouseholdEmissionAfterSaving = 0.0, totalHouseholdEmissionSaving = 0.0;
	double wasteEmissionBeforeRecyle = 0.0, wasteEmissionAfterRecyle = 0.0, wateEmissionAluminium = 0.0, wasteEmissionPlastic = 0.0, wasteEmissionGlass = 0.0, wasteEmissionNewsPaper = 0.0, wasteEmissionMagazine = 0.0, totalWasteEmissionAfterSaving = 0.0, totalWasteEmissionSaving = 0.0;
	double computerEnergySaving = 0.0, dryerEnergySaving = 0.0, lampEnergySaving = 0.0, refrigeratorEnergySaving = 0.0;
	bool isFound = false;
	string state = "";
	double electricityEmissionFactor = 0.0;
	string zipCode = "";
	cout << "*********************Your Current household emission**************************\n";
	cout << endl;
	cout << "----General information:----" <<endl <<endl;
	cout << "Total members in your family: ";
	cin >> totalFamilyMember;
	while (cin.fail() || totalFamilyMember < 0 ){ //check for a valid number input
		int func = CheckValidInput(totalFamilyMember, "Please enter a valid number for family members: ");
	} 

	cout << "Zip code (5-digits): ";
	cin >> zipCode;
	cout << "\n";
	
	//The ReadCSVFile() function read a predefined .csv file
	//This .csv file contains the ZIP codes, State codes and CO2 emission factors for different state
	//return value is a vector

	vector<vector<string>> CSVFileEmissionFactor  = ReadCSVFile();
	//Check the CSV file was not found or empty; the program needed the CSV file information
	if (CSVFileEmissionFactor.empty())
	{
		cout << "The CSV file contains (zip code, state code, electricity emission factor) was not found";
		return 0;
	}
	// Search the user zip code in the CSV file's values
	isFound = SearchZipCode(CSVFileEmissionFactor, zipCode, state, electricityEmissionFactor);
	// If zip code is not found, asked for zip code again and search it
	while (!isFound)
	{
		cout << "Please enter a valid zip code with length 5-digits: " ;
		cin >> zipCode;
		isFound = SearchZipCode(CSVFileEmissionFactor, zipCode, state, electricityEmissionFactor);
	}
	
	cout << endl << endl;
	//Start the household energy emission calculation
	cout << "----Home Energy's Emission Calculation----"<< endl << endl;

	cout << "Enter average monthly natural gas bill ($): ";
	cin >> monthlyGasBill;
	while (cin.fail() || monthlyGasBill < 0) {
		int func = CheckValidInput(monthlyGasBill, "Please enter a valid number for gas bill ($): ");
	}
	//emissions of natural gas =(average monthly gas bill per month / price per thousand cubic feet) * emission factor of a natural gas * months in a year
	gasEmission = CalculatedEnergyEmission(monthlyGasBill, GAS_COST_THOUSAND_CUBIC_FEET, EMISSION_FACTOR_GAS, TOTAL_MONTH_OF_YEAR);
	cout << "Total equivalence CO2 Emission from natural gas: " << gasEmission << " (lbs)" << endl;

	cout << "Enter average monthly electricity bill ($): ";
	cin >> monthlyElectricityBill;
	while (cin.fail() || monthlyElectricityBill < 0) {
		int func = CheckValidInput(monthlyElectricityBill, "Please enter a valid number for electricity bill ($): ");
	}
	//emissions of electricity =  (average monthly electric bill / price per kWh) * electricity emission factor  * months in a year
	electricityEmission = CalculatedEnergyEmission(monthlyElectricityBill, COST_PER_KWH, electricityEmissionFactor, TOTAL_MONTH_OF_YEAR);
	cout << "Total equivalence CO2 Emission from electricity: " << electricityEmission << " (lbs)" << endl;


	cout << "Enter average monthly fuel oil bill ($): ";
	cin >> monthlyFuelOilBill;
	while (cin.fail() || monthlyFuelOilBill < 0) {
		int func = CheckValidInput(monthlyFuelOilBill, "Please enter a valid number for oil bill ($): ");
	}
	//emission of fuel oil = (average monthly fuel oil bill / price per gallon) * fuel oil emission factor * months in a year 
	fuelOilEmission = CalculatedEnergyEmission(monthlyFuelOilBill, FUEL_OIL_COST, EMISSION_FACTOR_FUEL_OIL, TOTAL_MONTH_OF_YEAR);
	cout << "Total equivalence CO2 Emission from fuel oil: " << fuelOilEmission << " (lbs)" << endl;

	cout << "Enter average monthly propane bill ($): ";
	cin >> monthlyPropaneBill;
	while (cin.fail() || monthlyPropaneBill < 0) {
		int func = CheckValidInput(monthlyPropaneBill, "Please enter a valid number for propane bill ($): ");
	}
	//emission of propane = (average monthly propane bill / price per gallon) * propane emission factor * months in a year 
	propaneEmission = CalculatedEnergyEmission(monthlyPropaneBill, PROPANE_COST, EMISSION_FACTOR_PROPANE, TOTAL_MONTH_OF_YEAR);
	cout << "Total equivalence CO2 Emission from propane: " << propaneEmission << " (lbs)" << endl << endl;
	//calculate total household emission from energy resources
	totalHouseholdEmissionBeforeSaving = gasEmission + electricityEmission + fuelOilEmission + propaneEmission;
	// calculate the energy savings from different household plans
	cout << "----Actions to take reduce household emission----"<< endl << endl;
	cout << "Enable sleep feature on your computer and monitor. Will you take this action? (1=yes, 2 = no) : ";
	cin >> reduceActionOrNot;
	while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
		int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
	}
	if (reduceActionOrNot == 1) {
		//Emission reduction = If action is selelcted :
		//annual energy savings from enabling sleep feature on computer and monitor * electricity emission factor
		computerEnergySaving = EnergyEmissionSaving(COMPUTER_SLEEP_SAVING, electricityEmissionFactor);
		cout << "Emission saving from computer and monitor: " << computerEnergySaving << " (lbs)" << endl;
	}
	cout << "Use a clothes line or drying rack for 50 % of your laundry, instead of your dryer. Will you take this action? (1=yes, 2 = no) : ";
	cin >> reduceActionOrNot;
	while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
		int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
	}
	if (reduceActionOrNot == 1) {
		//Emission reduction = If action is selelcted :
		//annual energy savings from enabling to reduce 50% drayer loading is equivalence to (drayer energy/2) * electricity emission factor
		// As it is 50% that's why DRYER_ENERGY was divided by 2
		dryerEnergySaving = EnergyEmissionSaving(DRYER_ENERGY/2, electricityEmissionFactor);
		cout << "Emission saving from drayer: " << dryerEnergySaving << " (lbs)" <<endl;
	}
	cout << "Replace 60-watt incandescent light bulbs with 13-watt ENERGY STAR lights. Will you take this action? (1=yes, 2 = no) :";
	cin >> reduceActionOrNot;
	while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
		int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
	}
	if (reduceActionOrNot == 1) {
		//Emission reduction = If action is selected :
		//number of 60 - watt incandescent light bulbs replaced * annual kwh savings per lamp * electricity emission factor
		cout << "How many bulbs do you want to replace?: ";
		cin >> numberofBulbs;
		while (cin.fail() || numberofBulbs < 0) {
			int func = CheckValidInput(numberofBulbs, "Please enter a valid number for bulbs) : ");
		}
		lampEnergySaving = EnergyEmissionSaving(LAMP_KWH_SAVING*numberofBulbs, electricityEmissionFactor);
		cout << "Emission saving from bulbs: " << lampEnergySaving << " (lbs)" << endl;
	}

	cout << "Replace your household's old refrigerator with an ENERGY STAR model. Will you take this action? (1=yes, 2 = no) : ";
	cin >> reduceActionOrNot;
	while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
		int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
	}
	if (reduceActionOrNot == 1) {
		//Emission reduction = If action is selected :
		//refriderator replace emission saving * electricity emission factor
		refrigeratorEnergySaving = EnergyEmissionSaving(FRIDGE_KWH_SAVING, electricityEmissionFactor);
		cout << "Emission saving from refrigerator: " << refrigeratorEnergySaving << " (lbs)" << endl << endl;
	}
	//calculate the total household emission savings from the household energy
	totalHouseholdEmissionSaving = computerEnergySaving + dryerEnergySaving + lampEnergySaving + refrigeratorEnergySaving;
	//calculate total household energy emissions after savings
	totalHouseholdEmissionAfterSaving = totalHouseholdEmissionBeforeSaving - totalHouseholdEmissionSaving;
	//start vehicle emission calculation
	cout << "----Vehicle's Emission Calculation----"<<endl << endl;
	cout << "Enter number of vehicles that you have: ";
	cin >> numberOfVehicle;
	while (cin.fail() || numberOfVehicle < 0 ) {
		int func = CheckValidInput(numberOfVehicle, "Please enter a valid number for the number of vehicle : ");
	}
	//loop through the total number of vehicle and calculate the vehicle emission
	for (int i = 1; i <= numberOfVehicle; i++)
	{
		cout << "Total mileages are driven the vehicle " << i <<" per year: ";
		cin >> drivenMilesPerYear;
		while (cin.fail() || drivenMilesPerYear < 0) {
			int func = CheckValidInput(drivenMilesPerYear, "Please enter a valid number for total mileage for the vehicle :");
		}
		cout << "Total mileages are driven the vehicle " << i << " per gallon: ";
		cin >> milagesPerGallon;
		while (cin.fail() || milagesPerGallon < 0) {
			int func = CheckValidInput(milagesPerGallon, "Please enter a valid number for total mileage per gallon : ");
		}
		calculatedVehicleEmission = CalculateVehicleEmission(drivenMilesPerYear, milagesPerGallon, EMISSION_FACTOR_PASSENGER_VEHICLE, NON_CO2_VEHICLE_EMISSION_RATIO); // function returns the calculated emissions
		cout << "Emission produced by the vehicle " << i << ":  " << calculatedVehicleEmission << endl;
		totalVehicleEmissionBeforeSaving = totalVehicleEmissionBeforeSaving + calculatedVehicleEmission; //calculate total vehicle emission
	}
	cout << endl << endl;
	//vehicle emissions saving option
	if (numberOfVehicle > 0)
	{
		cout << "----Actions to take reduce the vehicles emission----" << endl << endl;
		cout << "Reduce total mileages of driven per year. Will you take this action? (1=yes, 2 = no) : ";
		cin >> reduceActionOrNot;
		while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
			int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
		}
		if (reduceActionOrNot == 1)
		{
			//loop through the total number of vehicle and calculate the emission saving
			for (int i = 1; i <= numberOfVehicle; i++)
			{
				cout << "Total mileages are reduced for the vehicle " << i << " per year ";
				cin >> drivenMilesPerYear;
				while (cin.fail() || drivenMilesPerYear < 0) {
					int func = CheckValidInput(drivenMilesPerYear, "Please enter a valid number for total mileage for the vehicle :");
				}
				cout << "Miles driven the vehicle " << i << " per gallon: ";
				cin >> milagesPerGallon;
				while (cin.fail() || milagesPerGallon < 0) {
					int func = CheckValidInput(milagesPerGallon, "Please enter a valid number for total mileage per gallon : ");
				}
				calculatedVehicleEmission = CalculateVehicleEmission(drivenMilesPerYear, milagesPerGallon, EMISSION_FACTOR_PASSENGER_VEHICLE, NON_CO2_VEHICLE_EMISSION_RATIO); // this function is also calculate the emission savings
				cout << "Emission reduced by the vehicle " << i << ":  " << calculatedVehicleEmission << endl;
				totalVehicleEmissionSaving = totalVehicleEmissionSaving + calculatedVehicleEmission; // calculate total emission savings from vehicle
			}
		}
	}
	totalVehicleEmissionAfterSaving = totalVehicleEmissionBeforeSaving - totalVehicleEmissionSaving; // calculate the vehicle emission after savings
	cout << endl;
	cout << "----Waste's Emission Calculation----"<< endl << endl;
	cout << "Total waste emissions before recycling ";
	//Emissions = number of people in household * average lb CO2 equivalent generated from waste per person per year (692 lbs)
	wasteEmissionBeforeRecyle = CalculatedWasteEmission(totalFamilyMember, AVERAGE_WASTE_EMISSION_PER_PERSON); // the function performs the waste emission calculation
	cout << wasteEmissionBeforeRecyle <<" (lbs)" << endl;
	cout << "Do you recycle aluminum and steel cans? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles metal, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by aluminum and steel cans
		wateEmissionAluminium = CalculatedWasteEmission(totalFamilyMember, METAL_RECYCLE_AVOID_EMISSION); 
		cout << "Waste emission saved from the aluminum and steel cans per year: " << wateEmissionAluminium << " (lbs)" << endl;
	}
	cout << "Do you recycle plastic? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles plastic, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by recycling plastic
		wasteEmissionPlastic = CalculatedWasteEmission(totalFamilyMember, PLASTIC_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the plastic per year: " << wasteEmissionPlastic << " (lbs)" << endl;
	}
	cout << "Do you recycle glass? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles glass, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by recycling glass
		wasteEmissionGlass = CalculatedWasteEmission(totalFamilyMember, GLASS_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the glass per year: " << wasteEmissionGlass << " (lbs)" << endl;
	}

	cout << "Do you recycle newspaper? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles newspaper, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by recycling newspaper
		wasteEmissionNewsPaper = CalculatedWasteEmission(totalFamilyMember, NEWSPAPER_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the newspaper per year: " << wasteEmissionNewsPaper << " (lbs)" << endl;
	}

	cout << "Do you recycle magazine? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles magazines, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by recycling magazine
		wasteEmissionMagazine = CalculatedWasteEmission(totalFamilyMember, MAGAZINE_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the magazine per year: " << wasteEmissionMagazine << " (lbs)" << endl;
	}
	wasteEmissionAfterRecyle = wasteEmissionBeforeRecyle - (wateEmissionAluminium + wasteEmissionPlastic + wasteEmissionGlass + wasteEmissionNewsPaper + wasteEmissionMagazine); // calculate waste emission after recycling
	cout << endl;
	// Waste emissions saving options
	cout << "----Actions to Take Waste's Emission Calculation----"<< endl << endl;

	cout << " Are you willing to start recycling the aluminum and steel cans? (1 = yes, 2 = already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles metal, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by aluminum and steel cans
		wateEmissionAluminium = CalculatedWasteEmission(totalFamilyMember, METAL_RECYCLE_AVOID_EMISSION); // same functon used for saving as well
		cout << "Waste emission saved from the aluminum and steel cans per year: " << wateEmissionAluminium << " (lbs)\n";
	}
	cout << " Are you willing to start recycling the plastic? (1=yes, 2=already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles plastic, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by recycling plastic
		wasteEmissionPlastic = CalculatedWasteEmission(totalFamilyMember, PLASTIC_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the plastic per year: " << wasteEmissionPlastic << " (lbs)\n";
	}
	cout << " Are you willing to start recycle the glass? (1=yes, 2=already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles glass, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by recycling glass
		wasteEmissionGlass = CalculatedWasteEmission(totalFamilyMember, GLASS_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the glass per year: " << wasteEmissionGlass << " (lbs)\n";
	}

	cout << " Are you willing to start recycle the newspaper? (1=yes, 2=already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles newspaper, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by recycling newspaper
		wasteEmissionNewsPaper = CalculatedWasteEmission(totalFamilyMember, NEWSPAPER_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the newspaper per year: " << wasteEmissionNewsPaper << " (lbs)\n";
	}

	cout << " Are you willing to start recycle the magazine? (1=yes, 2=already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		//If your household recycles magazines, then, number of people in household * average number of pounds of CO2 equivalent per person per year that could be saved by recycling magazine
		wasteEmissionMagazine = CalculatedWasteEmission(totalFamilyMember, MAGAZINE_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the magazine per year: " << wasteEmissionMagazine << " (lbs)\n";
	}
	//calculate total saving from household waste
	totalWasteEmissionSaving = wateEmissionAluminium + wasteEmissionPlastic + wasteEmissionGlass + wasteEmissionNewsPaper + wasteEmissionMagazine;
	totalWasteEmissionAfterSaving = wasteEmissionAfterRecyle - totalWasteEmissionSaving; // calculate emission after savings
	cout << endl;
	// print the summary of carbon footprint
	cout << "----Summary of Your Carbon Footprint----Annual Equivalence CO2 Emissions (lbs.)----"<<endl <<endl;
	
	cout << "Total members: " << totalFamilyMember << " State: "<<state << " Zip-code: " << zipCode << endl;
	cout << "Carbon Emission (CO2) is from the Household Energies: " << totalHouseholdEmissionBeforeSaving << " (lbs)" << endl;
	cout << "Carbon Emission (CO2) is from the Household Vehicles: " << totalVehicleEmissionBeforeSaving << " (lbs)" << endl;
	cout << "Carbon Emission (CO2) is from the Household Wastes: " << wasteEmissionAfterRecyle << " (lbs)" << endl;
	cout << "Current total Carbon Emission (CO2) is from the Household: " << (totalHouseholdEmissionBeforeSaving + totalVehicleEmissionBeforeSaving + wasteEmissionAfterRecyle) << " (lbs)" << endl;
	cout << "Number of trees is required to absorb the total emission: " << round((totalHouseholdEmissionBeforeSaving + totalVehicleEmissionBeforeSaving + wasteEmissionAfterRecyle) / TREE_ABSORB_CO2) << endl << endl; // calculate total number of total trees that can absorb the emission 
	
	cout << "Carbon Emission (CO2) is Savings after Planning:"<<endl;
	cout << "Carbon Emission (CO2) is Savings from the Household Energies: " << totalHouseholdEmissionSaving << " (lbs)" << endl;
	cout << "Carbon Emission (CO2) is Savings from the Household Vehicles: " << totalVehicleEmissionSaving << " (lbs)" << endl;
	cout << "Carbon Emission (CO2) is Savings from the Household Wastes: " << totalWasteEmissionSaving << " (lbs)" << endl;
	cout << "Total Carbon Emission (CO2) is from the Household: " << (totalHouseholdEmissionSaving + totalVehicleEmissionSaving + totalWasteEmissionSaving) << " (lbs)" << endl;
	cout << "Total Savings are Equivalence to the number of trees: " << round((totalHouseholdEmissionSaving + totalVehicleEmissionSaving + totalWasteEmissionSaving) / TREE_ABSORB_CO2) << endl << endl;
	cout << "New Total Carbon Emission (CO2) is after taking the saving actions: " << (totalHouseholdEmissionAfterSaving + totalVehicleEmissionAfterSaving + totalWasteEmissionAfterSaving) << " (lbs)" << endl;
	cout << "Number of trees is required to absorb the new total emission: " << round((totalHouseholdEmissionAfterSaving + totalVehicleEmissionAfterSaving + totalWasteEmissionAfterSaving) / TREE_ABSORB_CO2) << endl << endl;
	// summary report saved in a .txt file
	int retValue = WriteSummaryReport(totalFamilyMember, state, zipCode, totalHouseholdEmissionBeforeSaving, totalVehicleEmissionBeforeSaving, wasteEmissionAfterRecyle, totalHouseholdEmissionSaving, totalVehicleEmissionSaving, totalWasteEmissionSaving, (totalHouseholdEmissionAfterSaving + totalVehicleEmissionAfterSaving + totalWasteEmissionAfterSaving), TREE_ABSORB_CO2);
	return 0;
}

