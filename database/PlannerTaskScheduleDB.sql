SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

CREATE DATABASE IF NOT EXISTS `PlannerTaskScheduleDB` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
USE `PlannerTaskScheduleDB`;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `UserProfile` (
    `idUserProfile` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `LastName` VARCHAR(45) NOT NULL,
    `FirstName` TINYTEXT NOT NULL,
    `MiddleInitial` TINYTEXT,
    PRIMARY KEY (`idUserProfile`, `LastName`),
    UNIQUE INDEX `idUserProfile_UNIQUE` (`idUserProfile`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Tasks` (
    `idTasks` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `CreatedBy` int UNSIGNED NOT NULL,
    `AsignedTo` int UNSIGNED NOT NULL,
    `Description` TINYTEXT NOT NULL,
    `ParentTask` int UNSIGNED DEFAULT NULL,
    `Status` int UNSIGNED DEFAULT NULL,
    `PercentageComplete` double NOT NULL,
    PRIMARY KEY (`idTasks`),
    UNIQUE INDEX `idTasks_UNIQUE` (`idTasks` ASC),
    INDEX `fk_Tasks_CreatedBy_idx` (`CreatedBy` ASC),
    INDEX `fk_Tasks_AsignedTo_idx` (`AsignedTo` ASC),
    CONSTRAINT `fk_Tasks_CreatedBy` FOREIGN KEY (`CreatedBy`) REFERENCES `UserProfile` (`idUserProfile`),
    CONSTRAINT `fk_Tasks_AsignedTo` FOREIGN KEY (`AsignedTo`) REFERENCES `UserProfile` (`idUserProfile`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Dates` (
    `idDates` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` int UNSIGNED NOT NULL,
    `CreatedOn` date NOT NULL,
    `RequiredDelivery` date NOT NULL,
    `ScheduledStart` date NOT NULL,
    `ActualStart` date DEFAULT NULL,
    `EstimatedCompletion` date DEFAULT NULL,
    `Comleted` date DEFAULT NULL,
    PRIMARY KEY (`idDates`, `TaskID`),
    UNIQUE INDEX `idDates_UNIQUE` (`idDates` ASC),
    INDEX `fk_Dates_TaskID_idx` (`TaskID` ASC),
    CONSTRAINT `fk_Dates_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Effort` (
    `idEffort` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` int UNSIGNED NOT NULL,
    `EstimatedEffortHours` int UNSIGNED NOT NULL,
    `ActualEffortHours` float NOT NULL,
    PRIMARY KEY (`idEffort`, `TaskID`),
    UNIQUE INDEX `idEffort_UNIQUE` (`idEffort`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID`),
    CONSTRAINT `fk_Effort_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Goals` (
    `idGoals` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` int UNSIGNED NOT NULL,
    `Description` TINYTEXT NOT NULL,
    `Priority` int NOT NULL,
    `ParentGoal` int UNSIGNED DEFAULT NULL,
    PRIMARY KEY (`idGoals`, `UserID`),
    UNIQUE INDEX `idGoals_UNIQUE` (`idGoals`),
    CONSTRAINT `fk_Goals_UserID` FOREIGN KEY (`UserID`) REFERENCES `UserProfile` (`idUserProfile`),
    INDEX `fk_Goals_UserID_idx` (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `LoginAndPassword` (
    `idLoginAndPassword` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` int UNSIGNED NOT NULL,
    `LoginName` VARCHAR(45) NOT NULL,
    `HashedPassWord` TINYTEXT,
    PRIMARY KEY (`idLoginAndPassword`, `UserID`, `LoginName`),
    UNIQUE INDEX `idLoginAndPassword_UNIQUE` (`idLoginAndPassword` ASC),
    UNIQUE INDEX `UserID_UNIQUE` (`UserID` ASC),
    UNIQUE INDEX `LoginName_UNIQUE` (`LoginName` ASC),
    CONSTRAINT `fk_PassWord_UserID` FOREIGN KEY (`UserID`) REFERENCES `UserProfile` (`idUserProfile`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Priority` (
    `idPriority` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` int UNSIGNED NOT NULL,
    `Level` int UNSIGNED NOT NULL,
    `PriorityInLevel` int UNSIGNED NOT NULL,
    PRIMARY KEY (`idPriority`, `TaskID`),
    UNIQUE INDEX `idPriority_UNIQUE` (`idPriority` ASC),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID` ASC),
    CONSTRAINT `fk_Priority_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `UserTaskGoals` (
    `idUserTaskGoals` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `AssignedTo` int UNSIGNED NOT NULL,
    `TaskID`  int UNSIGNED NOT NULL,
    `Goal1` int UNSIGNED NOT NULL,
    `Goal2` int UNSIGNED DEFAULT NULL,
    `Goal3` int UNSIGNED DEFAULT NULL,
    `Goal4` int UNSIGNED DEFAULT NULL,
    PRIMARY KEY (`idUserTaskGoals`,`AssignedTo`,`TaskID`),
    CONSTRAINT `fk_UserTaskGoals_AsignedTo` FOREIGN KEY (`AssignedTo`) REFERENCES `UserProfile` (`idUserProfile`),
    CONSTRAINT `fk_UserTaskGoals_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `TaskStatus` (
    `idTaskStatus` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `StatusStr` VARCHAR(45) NOT NULL,
    PRIMARY KEY (`idTaskStatus`),
    UNIQUE INDEX `idTaskStatus_UNIQUE` (`idTaskStatus` ASC),
    UNIQUE INDEX `StatusStr_UNIQUE` (`StatusStr` ASC)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

INSERT INTO TaskStatus
    (StatusStr)
    VALUES
        ('Not Started'),
        ('On Hold'),
        ('Waiting for Dependency'),
        ('Work in Progress'),
        ('Complete');

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Dependecies` (
    `idDependecies` int UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID`  int UNSIGNED NOT NULL,
    `Dependency`  int UNSIGNED NOT NULL,
    PRIMARY KEY (`idDependecies`, `TaskID`),
    UNIQUE INDEX `idDependecies_UNIQUE` (`idDependecies` ASC),
    CONSTRAINT `fk_Dependecies_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
