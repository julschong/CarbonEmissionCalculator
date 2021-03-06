/* This program performs the carbon footprint calculation for a household
*Authors: Khadiza Akter, Meng Huan Lee, Alonso Ibarra
*Class: CIS-5-48643
*Date: 12/09/21
*/
#include <iostream>
#include "Util.h"
#include "Constants.h"
#include "Calculations.h"

using namespace std;

// ============================= Program entry point =============================
int main()
{
	int totalFamilyMember = 0, recyleOrNot = 0, reduceActionOrNot = 0, numberofBulbs = 0;
	string zipCode = "";
	string state = "";

	double monthlyGasBill = 0.0, monthlyElectricityBill = 0.0, monthlyFuelOilBill = 0.0, monthlyPropaneBill = 0.0;

	double gasEmission = 0.0, electricityEmission = 0.0, fuelOilEmission = 0.0,
		propaneEmission = 0.0, totalHouseholdEmissionBeforeSaving = 0.0,
		totalHouseholdEmissionAfterSaving = 0.0, totalHouseholdEmissionSaving = 0.0;
	int numberOfVehicle = 0;
	double drivenMilesPerYear = 0.0, milagesPerGallon = 0.0,
		totalVehicleEmissionBeforeSaving = 0.0, totalVehicleEmissionAfterSaving = 0.0,
		calculatedVehicleEmission = 0.0, totalVehicleEmissionSaving = 0.0;

	double wasteEmissionBeforeRecyle = 0.0, wasteEmissionAfterRecyle = 0.0,
		wateEmissionAluminium = 0.0, wasteEmissionPlastic = 0.0,
		wasteEmissionGlass = 0.0, wasteEmissionNewsPaper = 0.0,
		wasteEmissionMagazine = 0.0, totalWasteEmissionAfterSaving = 0.0, totalWasteEmissionSaving = 0.0;

	double computerEnergySaving = 0.0, dryerEnergySaving = 0.0,
		lampEnergySaving = 0.0, refrigeratorEnergySaving = 0.0;

	bool isFound = false;
	double electricityEmissionFactor = 0.0;

	//	============================= Print title and description =============================
	cout << "*********************Your Current household emission**************************\n";
	cout << endl;
	cout << "----General information:----" << endl << endl;
	
	//	============================= Gathering general user info =============================
	cout << "Total members in your family: ";
	cin >> totalFamilyMember;
	while (cin.fail() || totalFamilyMember < 0) { //check for a valid number input
		int func = CheckValidInput(totalFamilyMember, "Please enter a valid number for family members: ");
	}

	cout << "Zip code (5-digits): ";
	cin >> zipCode;
	cout << endl;


	//  ============================= Read CSV file =============================
	//	The ReadCSVFile() function read a predefined .csv file
	//	This .csv file contains the ZIP codes, State codes and CO2 emission factors for different state
	//	return value is a vector
	vector<vector<string>> CSVFileEmissionFactor = ReadCSVFile();

	//	Check the CSV file was not found or empty; the program needed the CSV file information
	if (CSVFileEmissionFactor.empty())
	{
		cout << "The CSV file contains (zip code, state code, electricity emission factor) was not found";
		return 0;
	}

	//	Search the user zip code in the CSV file's values
	isFound = SearchZipCode(CSVFileEmissionFactor, zipCode, state, electricityEmissionFactor);
	
	//	If zip code is not found, asked for zip code again and search it
	while (!isFound)
	{
		cout << "Please enter a valid zip code with length 5-digits: ";
		cin >> zipCode;
		isFound = SearchZipCode(CSVFileEmissionFactor, zipCode, state, electricityEmissionFactor);
	}
	cout << endl << endl;


	//	============================= Start the household energy emission calculation =============================
	cout << "----Home Energy's Emission Calculation----" << endl << endl;

	cout << "Enter average monthly natural gas bill ($): ";
	cin >> monthlyGasBill;
	while (cin.fail() || monthlyGasBill < 0) {
		int func = CheckValidInput(monthlyGasBill, "Please enter a valid number for gas bill ($): ");
	}

	//	Emissions of natural gas = (average monthly gas bill per month / price per thousand cubic feet) * 
	//									emission factor of a natural gas * months in a year
	gasEmission = CalculatedEnergyEmission(monthlyGasBill, GAS_COST_THOUSAND_CUBIC_FEET, EMISSION_FACTOR_GAS, TOTAL_MONTH_OF_YEAR);
	cout << "Total equivalence CO2 Emission from natural gas: " << gasEmission << " (lbs)" << endl;

	cout << "Enter average monthly electricity bill ($): ";
	cin >> monthlyElectricityBill;
	while (cin.fail() || monthlyElectricityBill < 0) {
		int func = CheckValidInput(monthlyElectricityBill, "Please enter a valid number for electricity bill ($): ");
	}

	//	Emissions of electricity =  (average monthly electric bill / price per kWh) * 
	//									electricity emission factor  * months in a year
	electricityEmission = CalculatedEnergyEmission(monthlyElectricityBill, COST_PER_KWH, electricityEmissionFactor, TOTAL_MONTH_OF_YEAR);
	cout << "Total equivalence CO2 Emission from electricity: " << electricityEmission << " (lbs)" << endl;


	cout << "Enter average monthly fuel oil bill ($): ";
	cin >> monthlyFuelOilBill;
	while (cin.fail() || monthlyFuelOilBill < 0) {
		int func = CheckValidInput(monthlyFuelOilBill, "Please enter a valid number for oil bill ($): ");
	}

	//	Emission of fuel oil = (average monthly fuel oil bill / price per gallon) * 
	//								fuel oil emission factor * months in a year 
	fuelOilEmission = CalculatedEnergyEmission(monthlyFuelOilBill, FUEL_OIL_COST, EMISSION_FACTOR_FUEL_OIL, TOTAL_MONTH_OF_YEAR);
	cout << "Total equivalence CO2 Emission from fuel oil: " << fuelOilEmission << " (lbs)" << endl;

	cout << "Enter average monthly propane bill ($): ";
	cin >> monthlyPropaneBill;
	while (cin.fail() || monthlyPropaneBill < 0) {
		int func = CheckValidInput(monthlyPropaneBill, "Please enter a valid number for propane bill ($): ");
	}

	//	Emission of propane = (average monthly propane bill / price per gallon) * 
	//							propane emission factor * months in a year 
	propaneEmission = CalculatedEnergyEmission(monthlyPropaneBill, PROPANE_COST, EMISSION_FACTOR_PROPANE, TOTAL_MONTH_OF_YEAR);
	cout << "Total equivalence CO2 Emission from propane: " << propaneEmission << " (lbs)" << endl << endl;
	
	//	Calculate total household emission from energy resources
	totalHouseholdEmissionBeforeSaving = gasEmission + electricityEmission + fuelOilEmission + propaneEmission;
	

	//	============================= Calculate the energy savings from different household plans =============================
	cout << "----Actions to take reduce household emission----" << endl << endl;
	
	//	Emission reduction = If action is selelcted :
	//	Annual energy savings from enabling sleep feature on computer and monitor * electricity emission factor
	cout << "Enable sleep feature on your computer and monitor. Will you take this action? (1=yes, 2 = no) : ";
	cin >> reduceActionOrNot;
	while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
		int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
	}
	if (reduceActionOrNot == 1) {
		computerEnergySaving = EnergyEmissionSaving(COMPUTER_SLEEP_SAVING, electricityEmissionFactor);
		cout << "Emission saving from computer and monitor: " << computerEnergySaving << " (lbs)" << endl;
	}

	//	Emission reduction = If action is selelcted :
	//	Annual energy savings from enabling to reduce 50% drayer loading is equivalence to (drayer energy/2) * electricity emission factor
	//	As it is 50% that's why DRYER_ENERGY was divided by 2
	cout << "Use a clothes line or drying rack for 50 % of your laundry, instead of your dryer. Will you take this action? (1=yes, 2 = no) : ";
	cin >> reduceActionOrNot;
	while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
		int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
	}
	if (reduceActionOrNot == 1) {
		dryerEnergySaving = EnergyEmissionSaving(DRYER_ENERGY / 2, electricityEmissionFactor);
		cout << "Emission saving from drayer: " << dryerEnergySaving << " (lbs)" << endl;
	}

	//	Emission reduction = If action is selected :
	//	Number of 60 - watt incandescent light bulbs replaced * annual kwh savings per lamp * electricity emission factor
	cout << "Replace 60-watt incandescent light bulbs with 13-watt ENERGY STAR lights. Will you take this action? (1=yes, 2 = no) :";
	cin >> reduceActionOrNot;
	while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
		int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
	}
	if (reduceActionOrNot == 1) {

		cout << "How many bulbs do you want to replace?: ";
		cin >> numberofBulbs;
		while (cin.fail() || numberofBulbs < 0) {
			int func = CheckValidInput(numberofBulbs, "Please enter a valid number for bulbs) : ");
		}
		lampEnergySaving = EnergyEmissionSaving(LAMP_KWH_SAVING * numberofBulbs, electricityEmissionFactor);
		cout << "Emission saving from bulbs: " << lampEnergySaving << " (lbs)" << endl;
	}

	//	Emission reduction = If action is selected :
	//	Refriderator replace emission saving * electricity emission factor
	cout << "Replace your household's old refrigerator with an ENERGY STAR model. Will you take this action? (1=yes, 2 = no) : ";
	cin >> reduceActionOrNot;
	while (cin.fail() || reduceActionOrNot <= 0 || reduceActionOrNot > 2) {
		int func = CheckValidInput(reduceActionOrNot, "Please enter a valid number for taking the action (1=yes, 2 = no) : ");
	}
	if (reduceActionOrNot == 1) {
		refrigeratorEnergySaving = EnergyEmissionSaving(FRIDGE_KWH_SAVING, electricityEmissionFactor);
		cout << "Emission saving from refrigerator: " << refrigeratorEnergySaving << " (lbs)" << endl << endl;
	}

	//	Calculate the total household emission savings from the household energy
	totalHouseholdEmissionSaving = computerEnergySaving + dryerEnergySaving + lampEnergySaving + refrigeratorEnergySaving;

	//	Calculate total household energy emissions after savings
	totalHouseholdEmissionAfterSaving = totalHouseholdEmissionBeforeSaving - totalHouseholdEmissionSaving;


	//	============================= Start vehicle emission calculation =============================
	cout << "----Vehicle's Emission Calculation----" << endl << endl;
	cout << "Enter number of vehicles that you have: ";
	cin >> numberOfVehicle;
	while (cin.fail() || numberOfVehicle < 0) {
		int func = CheckValidInput(numberOfVehicle, "Please enter a valid number for the number of vehicle : ");
	}
	//	Loop through the total number of vehicle and calculate the vehicle emission
	for (int i = 1; i <= numberOfVehicle; i++)
	{
		cout << "Total mileages are driven the vehicle " << i << " per year: ";
		cin >> drivenMilesPerYear;
		while (cin.fail() || drivenMilesPerYear < 0) {
			int func = CheckValidInput(drivenMilesPerYear, "Please enter a valid number for total mileage for the vehicle :");
		}
		cout << "Total mileages are driven the vehicle " << i << " per gallon: ";
		cin >> milagesPerGallon;
		while (cin.fail() || milagesPerGallon < 0) {
			int func = CheckValidInput(milagesPerGallon, "Please enter a valid number for total mileage per gallon : ");
		}

		//	Function returns the calculated emissions
		calculatedVehicleEmission = CalculateVehicleEmission(drivenMilesPerYear, milagesPerGallon, 
			EMISSION_FACTOR_PASSENGER_VEHICLE, NON_CO2_VEHICLE_EMISSION_RATIO);
		cout << "Emission produced by the vehicle " << i << ":  " << calculatedVehicleEmission << endl;

		//	Calculate total vehicle emission
		totalVehicleEmissionBeforeSaving = totalVehicleEmissionBeforeSaving + calculatedVehicleEmission; 
	}
	cout << endl << endl;

	//	Vehicle emissions saving option
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
			//	Loop through the total number of vehicle and calculate the emission saving
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
				totalVehicleEmissionSaving = totalVehicleEmissionSaving + calculatedVehicleEmission;
			}
		}
	}

	//	Calculate the vehicle emission after savings
	totalVehicleEmissionAfterSaving = totalVehicleEmissionBeforeSaving - totalVehicleEmissionSaving; 
	cout << endl;

	cout << "----Waste's Emission Calculation----" << endl << endl;
	cout << "Total waste emissions before recycling ";
	
	//	Emissions = number of people in household * average lb CO2 equivalent generated from waste per person per year (692 lbs)
	wasteEmissionBeforeRecyle = CalculatedWasteEmission(totalFamilyMember, AVERAGE_WASTE_EMISSION_PER_PERSON);
	cout << wasteEmissionBeforeRecyle << " (lbs)" << endl;
	

	// ============================= Recycle questionnaire =============================
	
	//	If your household recycles metal, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by aluminum and steel cans
	cout << "Do you recycle aluminum and steel cans? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wateEmissionAluminium = CalculatedWasteEmission(totalFamilyMember, METAL_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the aluminum and steel cans per year: " << wateEmissionAluminium << " (lbs)" << endl;
	}
	
	//	If your household recycles plastic, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by recycling plastic
	cout << "Do you recycle plastic? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wasteEmissionPlastic = CalculatedWasteEmission(totalFamilyMember, PLASTIC_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the plastic per year: " << wasteEmissionPlastic << " (lbs)" << endl;
	}
	
	//	If your household recycles glass, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by recycling glass
	cout << "Do you recycle glass? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wasteEmissionGlass = CalculatedWasteEmission(totalFamilyMember, GLASS_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the glass per year: " << wasteEmissionGlass << " (lbs)" << endl;
	}

	//	If your household recycles newspaper, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by recycling newspaper
	cout << "Do you recycle newspaper? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wasteEmissionNewsPaper = CalculatedWasteEmission(totalFamilyMember, NEWSPAPER_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the newspaper per year: " << wasteEmissionNewsPaper << " (lbs)" << endl;
	}

	//	If your household recycles magazines, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by recycling magazine
	cout << "Do you recycle magazine? (1=yes, 2 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 2) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action(1 = yes, 2 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wasteEmissionMagazine = CalculatedWasteEmission(totalFamilyMember, MAGAZINE_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the magazine per year: " << wasteEmissionMagazine << " (lbs)" << endl;
	}
	wasteEmissionAfterRecyle = wasteEmissionBeforeRecyle - (wateEmissionAluminium + wasteEmissionPlastic + wasteEmissionGlass + wasteEmissionNewsPaper + wasteEmissionMagazine); // calculate waste emission after recycling
	cout << endl;


	//	============================= Waste emissions saving options =============================

	cout << "----Actions to Take Waste's Emission Calculation----" << endl << endl;

	//	If your household recycles metal, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by aluminum and steel cans
	cout << " Are you willing to start recycling the aluminum and steel cans? (1 = yes, 2 = already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wateEmissionAluminium = CalculatedWasteEmission(totalFamilyMember, METAL_RECYCLE_AVOID_EMISSION); // same functon used for saving as well
		cout << "Waste emission saved from the aluminum and steel cans per year: " << wateEmissionAluminium << " (lbs)\n";
	}
	
	//	If your household recycles plastic, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by recycling plastic
	cout << " Are you willing to start recycling the plastic? (1=yes, 2=already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wasteEmissionPlastic = CalculatedWasteEmission(totalFamilyMember, PLASTIC_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the plastic per year: " << wasteEmissionPlastic << " (lbs)\n";
	}

	//	If your household recycles glass, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by recycling glass
	cout << " Are you willing to start recycle the glass? (1=yes, 2=already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wasteEmissionGlass = CalculatedWasteEmission(totalFamilyMember, GLASS_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the glass per year: " << wasteEmissionGlass << " (lbs)\n";
	}

	//	If your household recycles newspaper, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by recycling newspaper
	cout << " Are you willing to start recycle the newspaper? (1=yes, 2=already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, "Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wasteEmissionNewsPaper = CalculatedWasteEmission(totalFamilyMember, NEWSPAPER_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the newspaper per year: " << wasteEmissionNewsPaper << " (lbs)\n";
	}

	//	If your household recycles magazines, then, number of people in household * average number of pounds of CO2 
	//	equivalent per person per year that could be saved by recycling magazine
	cout << " Are you willing to start recycle the magazine? (1=yes, 2=already recycle, 3 = no): ";
	cin >> recyleOrNot;
	while (cin.fail() || recyleOrNot <= 0 || recyleOrNot > 3) {
		int func = CheckValidInput(recyleOrNot, 
			"Please enter a valid number for taking the action (1 = yes, 2 = already recycle, 3 = no) : ");
	}
	if (recyleOrNot == 1)
	{
		wasteEmissionMagazine = CalculatedWasteEmission(totalFamilyMember, MAGAZINE_RECYCLE_AVOID_EMISSION);
		cout << "Waste emission saved from the magazine per year: " << wasteEmissionMagazine << " (lbs)\n";
	}

	//	Calculate total saving from household waste
	totalWasteEmissionSaving = wateEmissionAluminium + wasteEmissionPlastic +
		wasteEmissionGlass + wasteEmissionNewsPaper + wasteEmissionMagazine;

	//	Calculate emission after savings
	totalWasteEmissionAfterSaving = wasteEmissionAfterRecyle - totalWasteEmissionSaving; 
	cout << endl;


	//	============================= Print the summary of carbon footprint =============================
	cout << "----Summary of Your Carbon Footprint----Annual Equivalence CO2 Emissions (lbs.)----" << endl << endl;

	cout << "Total members: " << totalFamilyMember << " State: " << state << " Zip-code: " << zipCode << endl
		<< "\tCarbon Emission (CO2) is from the Household Energies: " << totalHouseholdEmissionBeforeSaving << " (lbs)" << endl
		<< "\tCarbon Emission (CO2) is from the Household Vehicles: " << totalVehicleEmissionBeforeSaving << " (lbs)" << endl
		<< "\tCarbon Emission (CO2) is from the Household Wastes: " << wasteEmissionAfterRecyle << " (lbs)" << endl;
	
	cout << "Current total Carbon Emission (CO2) is from the Household: "
		<< (totalHouseholdEmissionBeforeSaving + totalVehicleEmissionBeforeSaving + wasteEmissionAfterRecyle) << " (lbs)" << endl
		<< "Number of trees is required to absorb the total emission: "
		<< round((totalHouseholdEmissionBeforeSaving + totalVehicleEmissionBeforeSaving + wasteEmissionAfterRecyle) / TREE_ABSORB_CO2) << endl << endl;

	//	Calculate total number of total trees that can absorb the emission 
	cout << "Carbon Emission (CO2) is Savings after Planning:" << endl
		<< "\tCarbon Emission (CO2) is Savings from the Household Energies: " << totalHouseholdEmissionSaving << " (lbs)" << endl
		<< "\tCarbon Emission (CO2) is Savings from the Household Vehicles: " << totalVehicleEmissionSaving << " (lbs)" << endl
		<< "\tCarbon Emission (CO2) is Savings from the Household Wastes: " << totalWasteEmissionSaving << " (lbs)" << endl;

	cout << "Total Carbon Emission (CO2) is from the Household: "
		<< (totalHouseholdEmissionSaving + totalVehicleEmissionSaving + totalWasteEmissionSaving) << " (lbs)" << endl
		<< "Total Savings are Equivalence to the number of trees: "
		<< round((totalHouseholdEmissionSaving + totalVehicleEmissionSaving + totalWasteEmissionSaving) / TREE_ABSORB_CO2) << endl << endl
		<< "New Total Carbon Emission (CO2) is after taking the saving actions: "
		<< (totalHouseholdEmissionAfterSaving + totalVehicleEmissionAfterSaving + totalWasteEmissionAfterSaving) << " (lbs)" << endl
		<< "Number of trees is required to absorb the new total emission: "
		<< round((totalHouseholdEmissionAfterSaving + totalVehicleEmissionAfterSaving + totalWasteEmissionAfterSaving) / TREE_ABSORB_CO2)
		<< endl << endl;

	//	Summary report saved in a .txt file
	int retValue = WriteSummaryReport(totalFamilyMember, state, zipCode,
		totalHouseholdEmissionBeforeSaving, totalVehicleEmissionBeforeSaving, wasteEmissionAfterRecyle,
		totalHouseholdEmissionSaving, totalVehicleEmissionSaving, totalWasteEmissionSaving,
		(totalHouseholdEmissionAfterSaving + totalVehicleEmissionAfterSaving + totalWasteEmissionAfterSaving), TREE_ABSORB_CO2);

	//	Pause before ending the program
	system("pause");

	return 0;
}

