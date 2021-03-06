
/* BiometricData
 * Armazena as biometrias dos usuários do sistema. Não há ligação com a tabela
 * dos usuários.
 */
DROP TABLE IF EXISTS BiometricData;
CREATE TABLE BiometricData (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        empRegId INTEGER CONSTRAINT fk_EmployeeReg_id REFERENCES EmployeeReg(id),
        nrFinger INTEGER NOT NULL,
	template BLOB NOT NULL
);

/* Configuration
 * Tabela responsável por armazenar configurações
 * 
 */
DROP TABLE IF EXISTS Configuration; 
CREATE TABLE Configuration (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	equipmentNumber INTEGER NOT NULL DEFAULT 1,
	ip VARCHAR(15) NULL NOT NULL DEFAULT '192.168.000.200',
	tpBiometric INTEGER NOT NULL,
	tpPaperCut INTEGER NOT NULL DEFAULT 0,
	tpCardReader INTEGER NOT NULL DEFAULT 0,
	volume INTEGER NOT NULL DEFAULT 10,
	tcpPort INTEGER NOT NULL DEFAULT 3000,
	usartBaudRate INTEGER NOT NULL DEFAULT 115200,
	nrRep VARCHAR(17) NOT NULL DEFAULT '',
	reportOnlineEvent INTEGER NOT NULL DEFAULT 0,
	reportOnlineStatus INTEGER NOT NULL DEFAULT 0,
	messageDisplay VARCHAR(255) NOT NULL DEFAULT '',
	verify1_N INTEGER NOT NULL DEFAULT 0,
	autoOn INTEGER NOT NULL DEFAULT 0,
	keyboardEnable INTEGER NOT NULL DEFAULT 0,
	tpCommunication INTEGER NOT NULL DEFAULT 1,
	connectionIp VARCHAR(15) NOT NULL DEFAULT '',
        connectionSecure INTEGER NOT NULL DEFAULT 1
);

/* DateTime
 * Tabela responsável por armazenar a data hora do sistema. 
 * Nela deverá existir uma trigger para informar alterações.
 * O sistema irá retornar a data hora dessa tabela e atualizar no banco
 *
 */
DROP TABLE IF EXISTS DateTime; 
CREATE TABLE DateTime (
  	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	dateTime TIMESTAMP NOT NULL
);

/* Employee
 * Tabela de armazenamento da lista de usuários.
 * 
 */

DROP TABLE IF EXISTS Employee; 
CREATE TABLE Employee (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	name VARCHAR(52) NOT NULL,
	pis VARCHAR(12) NOT NULL,
	verifyBioData INTEGER NOT NULL DEFAULT 0
);

DROP TABLE IF EXISTS EmployeeLog;
CREATE TABLE EmployeeLog (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	name VARCHAR(52) NOT NULL,
	pis VARCHAR(12) NOT NULL
);

DROP TABLE IF EXISTS EmployeeReg;
CREATE TABLE EmployeeReg(
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	empId INTEGER CONSTRAINT fk_Employee_id REFERENCES Employee(id),
	registration VARCHAR(20) NOT NULL
);

/* Employer
 * Tabela de armazenamento de empregador. Armazena as informações do 
 * empregador cadastrado no sistema 
 */
DROP TABLE IF EXISTS Employer; 
CREATE TABLE Employer (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	name VARCHAR(150) NOT NULL,
	document VARCHAR(18) NOT NULL,
	cei VARCHAR(12) NOT NULL DEFAULT '',
	docType INTEGER NULL DEFAULT 0,
	local VARCHAR(100) NOT NULL
);

/* Systemlog
 * Armazena logs gerados pelo sistema.
 * 
 */

DROP TABLE IF EXISTS SystemLog;
CREATE TABLE Systemlog (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	code INTEGER NULL,
	dateTime TIMESTAMP NOT NULL,
  	description VARCHAR(255) NULL,
        dataLog VARCHAR(120) NULL
);

/* User
 * Armazena os usuários do banco
 * Cada usuario possui um usuario e senha
 * Time stamp é atualizado a cada interação do usuario com as tabelas
 * ip é o endereço que o usuario acessou a ultima vez
 * ActiveSession é setado em logs no bd e zerado depois de X tempo
 * sem interação com a aplicação
 */
DROP TABLE IF EXISTS SysUser; 
CREATE TABLE SysUser (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	login VARCHAR(32) NOT NULL,
	password VARCHAR(32) NULL,
	dateTime TIMESTAMP NOT NULL,
	ip VARCHAR(15) NOT NULL DEFAULT '0.0.0.0',
  	activeSession INTEGER NOT NULL
);

/* SummerTime
 * Tradução ao pé da letra para horário de verão
 * é setado ao sistema uma data de inicio e fim para
 * o horário de verão
 */
DROP TABLE IF EXISTS SummerTime; 
CREATE TABLE SummerTime(
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	initiation TIMESTAMP NOT NULL,
	conclusion TIMESTAMP NOT NULL,
        active INTEGER NOT NULL DEFAULT 0
);

/* WlanConfiguration
 * Tabela responsável por armazenar configurações de wireless
 *
 */
DROP TABLE IF EXISTS WlanConfiguration;
CREATE TABLE WlanConfiguration (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	essid VARCHAR(255) NOT NULL DEFAULT '',
        gateway VARCHAR(15) NOT NULL DEFAULT '',
	dns VARCHAR(15) NOT NULL DEFAULT '',
	macAddress VARCHAR(17) NOT NULL DEFAULT '00:00:00:00:00:00',
	netmask VARCHAR(15) NOT NULL DEFAULT '',
        password VARCHAR(255) NOT NULL DEFAULT ''
);
/*
 *	TABELA		-	CÓDIGO			
 *	BiometricData   -	0
 *	Configuration	-	1
 * 	DateTime	-	2
 *	Employee	-	3
 *	EmployeeLog     -       4
 *	EmployeeReg     -       5
 *	Employer	-	6
 *	SystemLog	-	7
 *	SysUser 	-	8
 *      SummerTime      -       9
 *      WlanConfig      -       10
 */

/*Trigger que adiciona no log quando alguma operação de Usuario foi realizada*/
DROP TRIGGER IF EXISTS log_User;
CREATE TRIGGER log_User AFTER UPDATE ON SysUser
     BEGIN
     INSERT INTO SystemLog (code,datetime,description, dataLog)
               values (8,DATETIME('NOW'),"Usuario Modificado", new.login);
     END;

/*Trigger que adiciona no log quando um Empregado foi cadastrado*/
DROP TRIGGER IF EXISTS log_insertEmployee;
CREATE TRIGGER log_insertEmployee AFTER INSERT ON Employee
     BEGIN
     INSERT INTO SystemLog (code,datetime,description, dataLog)
               values (3,DATETIME('NOW'),"Empregado Inserido", new.pis);
     END;

/*Trigger que adiciona no log quando um Empregado foi removido*/
DROP TRIGGER IF EXISTS log_deleteEmployee;
CREATE TRIGGER log_deleteEmployee AFTER DELETE ON Employee
     BEGIN
     INSERT INTO SystemLog (code,datetime,description, dataLog)
               values (3,DATETIME('NOW'),"Empregado Deletado", old.pis);
     END;

/*Trigger que adiciona no log quando um Empregador foi inserido*/
DROP TRIGGER IF EXISTS log_insertEmployer;
CREATE TRIGGER log_insertEmployer AFTER INSERT ON Employer
     BEGIN
     INSERT INTO SystemLog (code,datetime,description, dataLog)
               values (6,DATETIME('NOW'),"Empregador Inserido", new.document);
     END;

/*Trigger que adiciona no log quando um Empregador foi alterado*/
DROP TRIGGER IF EXISTS log_updateEmployer;
CREATE TRIGGER log_updateEmployer AFTER UPDATE ON Employer
     BEGIN
     INSERT INTO SystemLog (code,datetime,description, dataLog)
               values (6,DATETIME('NOW'),"Empregador Alterado", new.document);
     END;

/*Trigger que adiciona no log quando um Empregador foi removido*/
DROP TRIGGER IF EXISTS log_deleteEmployer;
CREATE TRIGGER log_deleteEmployer AFTER DELETE ON Employer
     BEGIN
     INSERT INTO SystemLog (code,datetime,description, dataLog)
               values (6,DATETIME('NOW'),"Empregador Deletado", old.document);
     END;

/*Trigger que adiciona no log quando uma configuracao foi inserida*/
DROP TRIGGER IF EXISTS log_insertConfiguration;
CREATE TRIGGER log_insertConfiguration AFTER INSERT ON Configuration
     BEGIN
     INSERT INTO SystemLog (code,datetime,description)
               values (1,DATETIME('NOW'),"Config Inserida");
     END;

/*Trigger que adiciona no log quando uma Configuration foi alterada*/
DROP TRIGGER IF EXISTS log_updateConfiguration;
CREATE TRIGGER log_updateConfiguration AFTER UPDATE ON Configuration
     BEGIN
     INSERT INTO SystemLog (code,datetime,description)
               values (1,DATETIME('NOW'),"Config Alterada");
     END;

/*Trigger que adiciona no log quando uma Configuration foi removida*/
DROP TRIGGER IF EXISTS log_deleteConfiguration;
CREATE TRIGGER log_deleteConfiguration AFTER DELETE ON Configuration
     BEGIN
     INSERT INTO SystemLog (code,datetime,description)
               values (1,DATETIME('NOW'),"Config Deletada");
     END;

/*Trigger que adiciona no log quando uma DateTime foi alterada*/
DROP TRIGGER IF EXISTS log_updateDateTime;
CREATE TRIGGER log_updateDateTime AFTER UPDATE ON DateTime
     BEGIN
     INSERT INTO SystemLog (code,datetime,description)
               values (2,DATETIME('NOW'),"DateTime Alterada");
     END;

/*Trigger que adiciona no log quando uma DateTime foi inserida*/
DROP TRIGGER IF EXISTS log_insertDateTime;
CREATE TRIGGER log_insertDateTime AFTER INSERT ON DateTime
     BEGIN
     INSERT INTO SystemLog (code,datetime,description)
               values (2,DATETIME('NOW'),"DateTime Inserida");
     END;

/*Trigger que adiciona no log quando um SummerTime foi alterada*/
DROP TRIGGER IF EXISTS log_updateDateTime;
CREATE TRIGGER log_updateSummerTimee AFTER UPDATE ON SummerTime
     BEGIN
     INSERT INTO SystemLog (code,datetime,description)
               values (9,DATETIME('NOW'),"SummerTime Alterado");
     END;

/*Trigger que adiciona no log quando um WlanConfiguration foi alterada*/
DROP TRIGGER IF EXISTS log_updateWlanConfig;
CREATE TRIGGER log_updateWlanConfig AFTER UPDATE ON WlanConfiguration
     BEGIN
     INSERT INTO SystemLog (code,datetime,description)
               values (10,DATETIME('NOW'),"WlanConfiguration Alterado");
     END;


/*Trigger que garante somente um empregador*/
DROP TRIGGER IF EXISTS insert_Employer;
CREATE TRIGGER insert_Employer AFTER INSERT ON Employer
	BEGIN
	DELETE FROM Employer where id <> new.id;
	END;

/*Trigger que garante somente uma configuração no equipamento*/
DROP TRIGGER IF EXISTS insert_Configuration;
CREATE TRIGGER insert_Configuration AFTER INSERT ON Configuration
	BEGIN
	DELETE FROM Configuration where id <> new.id;
	END;

/*Trigger que garante somente um horário de verão no equipamento*/
DROP TRIGGER IF EXISTS insert_SummerTime;
CREATE TRIGGER insert_SummerTime BEFORE INSERT ON SummerTime
	BEGIN
	DELETE FROM SummerTime where id <> new.id;
	END;

/*Trigger que garante somente uma dataHora no equipamento*/
DROP TRIGGER IF EXISTS insert_DateTime;
CREATE TRIGGER insert_DateTime BEFORE INSERT ON DateTime
	BEGIN
	DELETE FROM DateTime where id <> new.id;
	END;

/*Trigger que garante somente um user no equipamento*/
DROP TRIGGER IF EXISTS insert_User;
CREATE TRIGGER insert_User BEFORE INSERT ON sysUser
	BEGIN
	DELETE FROM sysUser where id <> new.id;
	END;

/*Trigger que garante que não exitam registros invalidos na tabela EmployeeReg*/
DROP TRIGGER IF EXISTS delete_EmpReg;
CREATE TRIGGER delete_EmpReg AFTER DELETE ON Employee
	FOR each ROW
	BEGIN
	DELETE FROM EmployeeReg WHERE EmployeeReg.EmpId=old.id;
	END;

/*Trigger que garante que não exitam templates invalidos na tabela BiometricData*/
DROP TRIGGER IF EXISTS delete_BiometricData;
CREATE TRIGGER delete_BiometricData AFTER DELETE ON EmployeeReg
	FOR each ROW
	BEGIN
	DELETE FROM BiometricData WHERE BiometricData.EmpRegId=old.id;
	END;

/*Trigger que garante somente uma configuração no equipamento*/
DROP TRIGGER IF EXISTS insert_WlanConfiguration;
CREATE TRIGGER insert_WlanConfiguration AFTER INSERT ON WlanConfiguration
	BEGIN
	DELETE FROM WlanConfiguration where id <> new.id;
	END;

/*Trigger que garante que não exitam usuarios duplicados na tabela employeeLog*/
DROP TRIGGER IF EXISTS delete_EmployeeLog;
CREATE TRIGGER delete_EmployeeLog AFTER DELETE ON Employee
	FOR each ROW
	BEGIN
	DELETE FROM EmployeeLog WHERE EmployeeLog.pis=old.pis;
        INSERT INTO EmployeeLog (NAME, PIS) VALUES (old.name, old.pis);
	END;

INSERT INTO SysUser(login,password,dateTime,ip,activeSession) values ("admin", "", DATETIME('NOW'), "",  0);
INSERT INTO Configuration (equipmentNumber, tpBiometric, ip, tpPaperCut, tpCardReader, volume, usartBaudRate, tcpPort, nrRep, reportOnlineEvent, reportOnlineStatus, messageDisplay, verify1_N, autoOn, keyboardEnable, tpCommunication, connectionIp, connectionSecure) VALUES (1, 1, "192.168.000.200", 1, 0, 8, 115200, 8080, "00000000000000001", 0, 0, " -=  PRISMA  =- ", 1, 0, 1, 0, "", 0);
