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
    `idUserProfile` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `LastName` VARCHAR(45) NOT NULL,
    `FirstName` TINYTEXT NOT NULL,
    `MiddleInitial` TINYTEXT,
    PRIMARY KEY (`idUserProfile`, `LastName`),
    UNIQUE INDEX `idUserProfile_UNIQUE` (`idUserProfile`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Tasks` (
    `idTasks` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `CreatedBy` INT UNSIGNED NOT NULL,
    `AsignedTo` INT UNSIGNED NOT NULL,
    `Description` TINYTEXT NOT NULL,
    `ParentTask` INT UNSIGNED DEFAULT NULL,
    `Status` INT UNSIGNED DEFAULT NULL,
    `PercentageComplete` double NOT NULL,
    PRIMARY KEY (`idTasks`),
    UNIQUE INDEX `idTasks_UNIQUE` (`idTasks` ASC),
    INDEX `fk_Tasks_CreatedBy_idx` (`CreatedBy` ASC),
    INDEX `fk_Tasks_AsignedTo_idx` (`AsignedTo` ASC),
    CONSTRAINT `fk_Tasks_CreatedBy`
        FOREIGN KEY (`CreatedBy`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT `fk_Tasks_AsignedTo`
        FOREIGN KEY (`AsignedTo`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Dates` (
    `idDates` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` INT UNSIGNED NOT NULL,
    `CreatedOn` date NOT NULL,
    `RequiredDelivery` date NOT NULL,
    `ScheduledStart` date NOT NULL,
    `ActualStart` date DEFAULT NULL,
    `EstimatedCompletion` date DEFAULT NULL,
    `Comleted` date DEFAULT NULL,
    PRIMARY KEY (`idDates`, `TaskID`),
    UNIQUE INDEX `idDates_UNIQUE` (`idDates` ASC),
    INDEX `fk_Dates_TaskID_idx` (`TaskID` ASC),
    CONSTRAINT `fk_Dates_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Effort` (
    `idEffort` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` INT UNSIGNED NOT NULL,
    `EstimatedEffortHours` INT UNSIGNED NOT NULL,
    `ActualEffortHours` float NOT NULL,
    PRIMARY KEY (`idEffort`, `TaskID`),
    UNIQUE INDEX `idEffort_UNIQUE` (`idEffort`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID`),
    CONSTRAINT `fk_Effort_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Goals` (
    `idGoals` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `Description` TINYTEXT NOT NULL,
    `Priority` INT NOT NULL,
    `ParentGoal` INT UNSIGNED DEFAULT NULL,
    PRIMARY KEY (`idGoals`, `UserID`),
    UNIQUE INDEX `idGoals_UNIQUE` (`idGoals`),
    CONSTRAINT `fk_Goals_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX `fk_Goals_UserID_idx` (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `LoginAndPassword` (
    `idLoginAndPassword` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `LoginName` VARCHAR(45) NOT NULL,
    `HashedPassWord` TINYTEXT,
    PRIMARY KEY (`idLoginAndPassword`, `UserID`, `LoginName`),
    UNIQUE INDEX `idLoginAndPassword_UNIQUE` (`idLoginAndPassword` ASC),
    UNIQUE INDEX `UserID_UNIQUE` (`UserID` ASC),
    UNIQUE INDEX `LoginName_UNIQUE` (`LoginName` ASC),
    CONSTRAINT `fk_PassWord_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `Priority` (
    `idPriority` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` INT UNSIGNED NOT NULL,
    `Level` INT UNSIGNED NOT NULL,
    `PriorityInLevel` INT UNSIGNED NOT NULL,
    PRIMARY KEY (`idPriority`, `TaskID`),
    UNIQUE INDEX `idPriority_UNIQUE` (`idPriority` ASC),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID` ASC),
    CONSTRAINT `fk_Priority_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `UserTaskGoals` (
    `idUserTaskGoals` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `AssignedTo` INT UNSIGNED NOT NULL,
    `TaskID`  INT UNSIGNED NOT NULL,
    `Goal1` INT UNSIGNED NOT NULL,
    `Goal2` INT UNSIGNED DEFAULT NULL,
    `Goal3` INT UNSIGNED DEFAULT NULL,
    `Goal4` INT UNSIGNED DEFAULT NULL,
    PRIMARY KEY (`idUserTaskGoals`,`AssignedTo`,`TaskID`),
    CONSTRAINT `fk_UserTaskGoals_AsignedTo`
        FOREIGN KEY (`AssignedTo`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT `fk_UserTaskGoals_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE IF NOT EXISTS `TaskStatus` (
    `idTaskStatus` INT UNSIGNED NOT NULL AUTO_INCREMENT,
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
    `idDependecies` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID`  INT UNSIGNED NOT NULL,
    `Dependency`  INT UNSIGNED NOT NULL,
    PRIMARY KEY (`idDependecies`, `TaskID`),
    UNIQUE INDEX `idDependecies_UNIQUE` (`idDependecies` ASC),
    CONSTRAINT `fk_Dependecies_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlatformIndependentPreferences`;
CREATE TABLE IF NOT EXISTS `PlatformIndependentPreferences` (
    `idPlatformIndependentPreferences` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `UseLettersForPriorityLevel` BOOLEAN NOT NULL,
    `SeparatePriorityWithDoc` BOOLEAN NOT NULL,
    `ScheduleDayStart` TINYINT DEFAULT 8,
    `ScheduleDayEnd` TINYINT DEFAULT 17,
    PRIMARY KEY (`idPlatformIndependentPreferences`),
    UNIQUE INDEX `idPlatformIndependentPreferences_UNIQUE` (`idPlatformIndependentPreferences` ASC),
    UNIQUE INDEX `UserID_UNIQUE` (`UserID` ASC),
    CONSTRAINT `fk_PlatformIndependentPrefs_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `ScheduleItemType`;
CREATE TABLE IF NOT EXISTS `ScheduleItemType` (
    `idScheduleItemType` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `ScheduleItemTypeStr` VARCHAR(45) NOT NULL,
    PRIMARY KEY (`idScheduleItemType`),
    UNIQUE INDEX `idScheduleItemType_UNIQUE` (`idScheduleItemType` ASC),
    UNIQUE INDEX `ScheduleItemTypeStr_UNIQUE` (`ScheduleItemTypeStr` ASC)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

INSERT INTO ScheduleItemType
    (ScheduleItemTypeStr)
    VALUES
        ('Meeting'),
        ('Phone Call'),
        ('Task Execution'),
        ('Personal Appointment'),
        ('Personal Other');


-- --------------------------------------------------------

CREATE TABLE `PlannerTaskScheduleDB`.`DaySchedule` (
    `idDaySchedule` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `DateOfSchedule` DATE NOT NULL,
    `StartOfDay` TIME NOT NULL,
    `EndOfDay` TIME NOT NULL,
    `DailyGoals` VARCHAR(45) NULL,
    PRIMARY KEY (`idDaySchedule`),
    UNIQUE INDEX `idDaySchedule_UNIQUE` (`idDaySchedule` ASC) VISIBLE,
    INDEX `fk_DaySchedule_UserID_idx` (`UserID` ASC) VISIBLE,
    CONSTRAINT `fk_DaySchedule_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `PlannerTaskScheduleDB`.`UserProfile` (`idUserProfile`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

CREATE TABLE `PlannerTaskScheduleDB`.`ScheduleItem` (
    `idScheduleItem` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `StartDateTime` DATETIME NOT NULL,
    `EndDateTime` DATETIME NOT NULL,
    `ItemType` TINYINT NOT NULL,
    `PriorityMajor` TINYINT NOT NULL,
    `PriorityMinor` TINYINT NOT NULL,
    `Location` VARCHAR(45) NULL,
    PRIMARY KEY (`idScheduleItem`),
    UNIQUE INDEX `idScheduleItem_UNIQUE` (`idScheduleItem` ASC) VISIBLE,
    INDEX `fk_ScheduleItem_UserID_idx` (`UserID` ASC) VISIBLE,
    CONSTRAINT `fk_ScheduleItem_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `PlannerTaskScheduleDB`.`UserProfile` (`idUserProfile`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
    
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
