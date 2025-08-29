-- phpMyAdmin SQL Dump
-- version 5.1.1deb5ubuntu1
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Apr 18, 2025 at 08:49 AM
-- Server version: 8.0.41-0ubuntu0.22.04.1
-- PHP Version: 8.1.2-1ubuntu2.21

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `PlannerTaskScheduleDB`
--

DELIMITER $$
--
-- Functions
--
CREATE DEFINER=`pacman2`@`localhost` FUNCTION `findScheduleItemTypeLabelByValue` (`StatusKey` INT) RETURNS VARCHAR(20) CHARSET utf8mb4 BEGIN
    
    SET @ScheduleItemTypeLabel = ``;
    
    SELECT ScheduleItemType.StatusStr INTO @ScheduleItemTypeLabel
        FROM ScheduleItemType
        WHERE ScheduleItemType.idScheduleItemType = StatusStr;
    IF @TasScheduleItemTypeLabelkStatusKey IS NULL THEN
        SET @ScheduleItemTypeLabel = 'Not Started';
    END IF;

    RETURN @ScheduleItemTypeLabel;
    
END$$

CREATE DEFINER=`pacman2`@`localhost` FUNCTION `findScheduleItemTypeValueByLabel` (`StatusStr` VARCHAR(20)) RETURNS INT BEGIN
    
    SET @ScheduleItemTypeKey = 0;
    
    SELECT ScheduleItemType.idScheduleItemType INTO @ScheduleItemTypeKey
        FROM ScheduleItemType
        WHERE ScheduleItemType.StatusStr = StatusStr;
    IF @ScheduleItemTypeKey IS NULL THEN
        SET @ScheduleItemTypeKey = 0;
    END IF;

    RETURN @ScheduleItemTypeKey;
    
END$$

CREATE DEFINER=`pacman2`@`localhost` FUNCTION `findTaskStatusLabelByValue` (`StatusKey` INT) RETURNS VARCHAR(20) CHARSET utf8mb4 BEGIN
    
    SET @TaskStatusLabel = ``;
    
    SELECT TaskStatus.StatusStr INTO @TaskStatusLabel
        FROM TaskStatus
        WHERE TaskStatus.idTaskStatus = StatusStr;
    IF @TasTaskStatusLabelkStatusKey IS NULL THEN
        SET @TaskStatusLabel = 'Not Started';
    END IF;

    RETURN @TaskStatusLabel;
    
END$$

CREATE DEFINER=`pacman2`@`localhost` FUNCTION `findTaskStatusValueByLabel` (`StatusStr` VARCHAR(20)) RETURNS INT BEGIN
    
    SET @TaskStatusKey = 0;
    
    SELECT TaskStatus.idTaskStatus INTO @TaskStatusKey
        FROM TaskStatus
        WHERE TaskStatus.StatusStr = StatusStr;
    IF @TaskStatusKey IS NULL THEN
        SET @TaskStatusKey = 0;
    END IF;

    RETURN @TaskStatusKey;
    
END$$

CREATE DEFINER=`pacman2`@`localhost` FUNCTION `findUserIDKeyByFullName` (`LastName` VARCHAR(45), `FirstName` TINYTEXT, `MiddleInitial` TINYTEXT) RETURNS INT BEGIN

    SET @UserIDKey = 0;

    SELECT UserProfile.idUserProfile INTO @UserIDKey
        FROM UserProfile
        WHERE UserProfile.LastName = LastName AND
            UserProfile.FirstName = FirstName AND
            UserProfile.MiddleInitial = MiddleInitial;
    IF @UserIDKey IS NULL THEN
        SET @UserIDKey = 0;
    END IF;

    RETURN @UserIDKey;
    
END$$

CREATE DEFINER=`pacman2`@`localhost` FUNCTION `findUserIDKeyByLoginName` (`LoginName` VARCHAR(45)) RETURNS INT BEGIN

    SET @UserIDKey = 0;

    SELECT LoginAndPassword.UserID INTO @UserIDKey
        FROM LoginAndPassword
        WHERE LoginAndPassword.LoginName = LoginName;
    IF @UserIDKey IS NULL THEN
        SET @UserIDKey = 0;
    END IF;

    RETURN @UserIDKey;
    
END$$

CREATE DEFINER=`pacman2`@`localhost` FUNCTION `isValidLoginAndPassword` (`LoginName` VARCHAR(45), `HashedPassWord` TINYTEXT) RETURNS TINYINT BEGIN

    SET @UserIDKey = 0;
    SET @isValid = 1;

    SELECT LoginAndPassword.UserID INTO @UserIDKey
        FROM LoginAndPassword
        WHERE LoginAndPassword.LoginName = LoginName AND
            LoginAndPassword.HashedPassWord = HashedPassWord;

    IF @UserIDKey IS NULL THEN
        SET @isValid = 0;
    END IF;

    RETURN @isValid;

END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `Dates`
--

CREATE TABLE `Dates` (
  `idDates` int UNSIGNED NOT NULL,
  `TaskID` int UNSIGNED NOT NULL,
  `CreatedOn` date NOT NULL,
  `RequiredDelivery` date NOT NULL,
  `ScheduledStart` date NOT NULL,
  `ActualStart` date DEFAULT NULL,
  `EstimatedCompletion` date DEFAULT NULL,
  `Comleted` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `DaySchedule`
--

CREATE TABLE `DaySchedule` (
  `idDaySchedule` int UNSIGNED NOT NULL,
  `UserID` int UNSIGNED NOT NULL,
  `DateOfSchedule` date NOT NULL,
  `StartOfDay` time NOT NULL,
  `EndOfDay` time NOT NULL,
  `DailyGoals` varchar(45) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Dependencies`
--

CREATE TABLE `Dependencies` (
  `idDependencies` int UNSIGNED NOT NULL,
  `TaskID` int UNSIGNED NOT NULL,
  `Dependency` int UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Effort`
--

CREATE TABLE `Effort` (
  `idEffort` int UNSIGNED NOT NULL,
  `TaskID` int UNSIGNED NOT NULL,
  `EstimatedEffortHours` int UNSIGNED NOT NULL,
  `ActualEffortHours` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Goals`
--

CREATE TABLE `Goals` (
  `idGoals` int UNSIGNED NOT NULL,
  `UserID` int UNSIGNED NOT NULL,
  `Description` tinytext NOT NULL,
  `Priority` int NOT NULL,
  `ParentGoal` int UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `LoginAndPassword`
--

CREATE TABLE `LoginAndPassword` (
  `idLoginAndPassword` int UNSIGNED NOT NULL,
  `UserID` int UNSIGNED NOT NULL,
  `LoginName` varchar(45) NOT NULL,
  `HashedPassWord` tinytext
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Notes`
--

CREATE TABLE `Notes` (
  `idNotes` int UNSIGNED NOT NULL,
  `UserID` int UNSIGNED NOT NULL,
  `NotationDateTime` datetime NOT NULL,
  `Content` varchar(1024) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `PlatformIndependentPreferences`
--

CREATE TABLE `PlatformIndependentPreferences` (
  `idPlatformIndependentPreferences` int UNSIGNED NOT NULL,
  `UserID` int UNSIGNED NOT NULL,
  `UseLettersForPriorityLevel` tinyint(1) NOT NULL,
  `SeparatePriorityWithDoc` tinyint(1) NOT NULL,
  `ScheduleDayStart` tinyint DEFAULT '8',
  `ScheduleDayEnd` tinyint DEFAULT '17'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Priority`
--

CREATE TABLE `Priority` (
  `idPriority` int UNSIGNED NOT NULL,
  `TaskID` int UNSIGNED NOT NULL,
  `Level` int UNSIGNED NOT NULL,
  `PriorityInLevel` int UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `ScheduleItem`
--

CREATE TABLE `ScheduleItem` (
  `idScheduleItem` int UNSIGNED NOT NULL,
  `UserID` int UNSIGNED NOT NULL,
  `StartDateTime` datetime NOT NULL,
  `EndDateTime` datetime NOT NULL,
  `ItemType` tinyint NOT NULL,
  `PriorityMajor` tinyint NOT NULL,
  `PriorityMinor` tinyint NOT NULL,
  `Location` varchar(45) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `ScheduleItemType`
--

CREATE TABLE `ScheduleItemType` (
  `idScheduleItemType` int UNSIGNED NOT NULL,
  `ScheduleItemTypeStr` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `ScheduleItemType`
--

INSERT INTO `ScheduleItemType` (`idScheduleItemType`, `ScheduleItemTypeStr`) VALUES
(1, 'Meeting'),
(4, 'Personal Appointment'),
(5, 'Personal Other'),
(2, 'Phone Call'),
(3, 'Task Execution');

-- --------------------------------------------------------

--
-- Table structure for table `Tasks`
--

CREATE TABLE `Tasks` (
  `idTasks` int UNSIGNED NOT NULL,
  `CreatedBy` int UNSIGNED NOT NULL,
  `AsignedTo` int UNSIGNED NOT NULL,
  `Description` tinytext NOT NULL,
  `ParentTask` int UNSIGNED DEFAULT NULL,
  `Status` int UNSIGNED DEFAULT NULL,
  `PercentageComplete` double NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `TaskStatus`
--

CREATE TABLE `TaskStatus` (
  `idTaskStatus` int UNSIGNED NOT NULL,
  `StatusStr` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `TaskStatus`
--

INSERT INTO `TaskStatus` (`idTaskStatus`, `StatusStr`) VALUES
(5, 'Complete'),
(1, 'Not Started'),
(2, 'On Hold'),
(3, 'Waiting for Dependency'),
(4, 'Work in Progress');

-- --------------------------------------------------------

--
-- Table structure for table `UserProfile`
--

CREATE TABLE `UserProfile` (
  `idUserProfile` int UNSIGNED NOT NULL,
  `LastName` varchar(45) NOT NULL,
  `FirstName` tinytext NOT NULL,
  `MiddleInitial` tinytext
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `UserTaskGoals`
--

CREATE TABLE `UserTaskGoals` (
  `idUserTaskGoals` int UNSIGNED NOT NULL,
  `AssignedTo` int UNSIGNED NOT NULL,
  `TaskID` int UNSIGNED NOT NULL,
  `Goal1` int UNSIGNED NOT NULL,
  `Goal2` int UNSIGNED DEFAULT NULL,
  `Goal3` int UNSIGNED DEFAULT NULL,
  `Goal4` int UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `Dates`
--
ALTER TABLE `Dates`
  ADD PRIMARY KEY (`idDates`,`TaskID`),
  ADD UNIQUE KEY `idDates_UNIQUE` (`idDates`),
  ADD KEY `fk_Dates_TaskID_idx` (`TaskID`);

--
-- Indexes for table `DaySchedule`
--
ALTER TABLE `DaySchedule`
  ADD PRIMARY KEY (`idDaySchedule`),
  ADD UNIQUE KEY `idDaySchedule_UNIQUE` (`idDaySchedule`),
  ADD KEY `fk_DaySchedule_UserID_idx` (`UserID`);

--
-- Indexes for table `Dependencies`
--
ALTER TABLE `Dependencies`
  ADD PRIMARY KEY (`idDependencies`,`TaskID`),
  ADD UNIQUE KEY `idDependencies_UNIQUE` (`idDependencies`),
  ADD KEY `fk_Dependencies_TaskID` (`TaskID`);

--
-- Indexes for table `Effort`
--
ALTER TABLE `Effort`
  ADD PRIMARY KEY (`idEffort`,`TaskID`),
  ADD UNIQUE KEY `idEffort_UNIQUE` (`idEffort`),
  ADD UNIQUE KEY `TaskID_UNIQUE` (`TaskID`);

--
-- Indexes for table `Goals`
--
ALTER TABLE `Goals`
  ADD PRIMARY KEY (`idGoals`,`UserID`),
  ADD UNIQUE KEY `idGoals_UNIQUE` (`idGoals`),
  ADD KEY `fk_Goals_UserID_idx` (`UserID`);

--
-- Indexes for table `LoginAndPassword`
--
ALTER TABLE `LoginAndPassword`
  ADD PRIMARY KEY (`idLoginAndPassword`,`UserID`,`LoginName`),
  ADD UNIQUE KEY `idLoginAndPassword_UNIQUE` (`idLoginAndPassword`),
  ADD UNIQUE KEY `UserID_UNIQUE` (`UserID`),
  ADD UNIQUE KEY `LoginName_UNIQUE` (`LoginName`);

--
-- Indexes for table `Notes`
--
ALTER TABLE `Notes`
  ADD PRIMARY KEY (`idNotes`),
  ADD UNIQUE KEY `idNotes_UNIQUE` (`idNotes`),
  ADD KEY `fk_Notes_UserID_idx` (`UserID`);

--
-- Indexes for table `PlatformIndependentPreferences`
--
ALTER TABLE `PlatformIndependentPreferences`
  ADD PRIMARY KEY (`idPlatformIndependentPreferences`),
  ADD UNIQUE KEY `idPlatformIndependentPreferences_UNIQUE` (`idPlatformIndependentPreferences`),
  ADD UNIQUE KEY `UserID_UNIQUE` (`UserID`);

--
-- Indexes for table `Priority`
--
ALTER TABLE `Priority`
  ADD PRIMARY KEY (`idPriority`,`TaskID`),
  ADD UNIQUE KEY `idPriority_UNIQUE` (`idPriority`),
  ADD UNIQUE KEY `TaskID_UNIQUE` (`TaskID`);

--
-- Indexes for table `ScheduleItem`
--
ALTER TABLE `ScheduleItem`
  ADD PRIMARY KEY (`idScheduleItem`),
  ADD UNIQUE KEY `idScheduleItem_UNIQUE` (`idScheduleItem`),
  ADD KEY `fk_ScheduleItem_UserID_idx` (`UserID`);

--
-- Indexes for table `ScheduleItemType`
--
ALTER TABLE `ScheduleItemType`
  ADD PRIMARY KEY (`idScheduleItemType`),
  ADD UNIQUE KEY `idScheduleItemType_UNIQUE` (`idScheduleItemType`),
  ADD UNIQUE KEY `ScheduleItemTypeStr_UNIQUE` (`ScheduleItemTypeStr`);

--
-- Indexes for table `Tasks`
--
ALTER TABLE `Tasks`
  ADD PRIMARY KEY (`idTasks`),
  ADD UNIQUE KEY `idTasks_UNIQUE` (`idTasks`),
  ADD KEY `fk_Tasks_CreatedBy_idx` (`CreatedBy`),
  ADD KEY `fk_Tasks_AsignedTo_idx` (`AsignedTo`);

--
-- Indexes for table `TaskStatus`
--
ALTER TABLE `TaskStatus`
  ADD PRIMARY KEY (`idTaskStatus`),
  ADD UNIQUE KEY `idTaskStatus_UNIQUE` (`idTaskStatus`),
  ADD UNIQUE KEY `StatusStr_UNIQUE` (`StatusStr`);

--
-- Indexes for table `UserProfile`
--
ALTER TABLE `UserProfile`
  ADD PRIMARY KEY (`idUserProfile`,`LastName`),
  ADD UNIQUE KEY `idUserProfile_UNIQUE` (`idUserProfile`);

--
-- Indexes for table `UserTaskGoals`
--
ALTER TABLE `UserTaskGoals`
  ADD PRIMARY KEY (`idUserTaskGoals`,`AssignedTo`,`TaskID`),
  ADD KEY `fk_UserTaskGoals_AsignedTo` (`AssignedTo`),
  ADD KEY `fk_UserTaskGoals_TaskID` (`TaskID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `Dates`
--
ALTER TABLE `Dates`
  MODIFY `idDates` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `DaySchedule`
--
ALTER TABLE `DaySchedule`
  MODIFY `idDaySchedule` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Dependencies`
--
ALTER TABLE `Dependencies`
  MODIFY `idDependencies` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Effort`
--
ALTER TABLE `Effort`
  MODIFY `idEffort` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Goals`
--
ALTER TABLE `Goals`
  MODIFY `idGoals` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `LoginAndPassword`
--
ALTER TABLE `LoginAndPassword`
  MODIFY `idLoginAndPassword` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Notes`
--
ALTER TABLE `Notes`
  MODIFY `idNotes` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `PlatformIndependentPreferences`
--
ALTER TABLE `PlatformIndependentPreferences`
  MODIFY `idPlatformIndependentPreferences` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Priority`
--
ALTER TABLE `Priority`
  MODIFY `idPriority` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `ScheduleItem`
--
ALTER TABLE `ScheduleItem`
  MODIFY `idScheduleItem` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `ScheduleItemType`
--
ALTER TABLE `ScheduleItemType`
  MODIFY `idScheduleItemType` int UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `Tasks`
--
ALTER TABLE `Tasks`
  MODIFY `idTasks` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `TaskStatus`
--
ALTER TABLE `TaskStatus`
  MODIFY `idTaskStatus` int UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `UserProfile`
--
ALTER TABLE `UserProfile`
  MODIFY `idUserProfile` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `UserTaskGoals`
--
ALTER TABLE `UserTaskGoals`
  MODIFY `idUserTaskGoals` int UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `Dates`
--
ALTER TABLE `Dates`
  ADD CONSTRAINT `fk_Dates_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `DaySchedule`
--
ALTER TABLE `DaySchedule`
  ADD CONSTRAINT `fk_DaySchedule_UserID` FOREIGN KEY (`UserID`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `Dependencies`
--
ALTER TABLE `Dependencies`
  ADD CONSTRAINT `fk_Dependencies_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `Effort`
--
ALTER TABLE `Effort`
  ADD CONSTRAINT `fk_Effort_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `Goals`
--
ALTER TABLE `Goals`
  ADD CONSTRAINT `fk_Goals_UserID` FOREIGN KEY (`UserID`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `LoginAndPassword`
--
ALTER TABLE `LoginAndPassword`
  ADD CONSTRAINT `fk_PassWord_UserID` FOREIGN KEY (`UserID`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `Notes`
--
ALTER TABLE `Notes`
  ADD CONSTRAINT `fk_Notes_UserID` FOREIGN KEY (`UserID`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `PlatformIndependentPreferences`
--
ALTER TABLE `PlatformIndependentPreferences`
  ADD CONSTRAINT `fk_PlatformIndependentPrefs_UserID` FOREIGN KEY (`UserID`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `Priority`
--
ALTER TABLE `Priority`
  ADD CONSTRAINT `fk_Priority_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `ScheduleItem`
--
ALTER TABLE `ScheduleItem`
  ADD CONSTRAINT `fk_ScheduleItem_UserID` FOREIGN KEY (`UserID`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `Tasks`
--
ALTER TABLE `Tasks`
  ADD CONSTRAINT `fk_Tasks_AsignedTo` FOREIGN KEY (`AsignedTo`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  ADD CONSTRAINT `fk_Tasks_CreatedBy` FOREIGN KEY (`CreatedBy`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Constraints for table `UserTaskGoals`
--
ALTER TABLE `UserTaskGoals`
  ADD CONSTRAINT `fk_UserTaskGoals_AsignedTo` FOREIGN KEY (`AssignedTo`) REFERENCES `UserProfile` (`idUserProfile`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  ADD CONSTRAINT `fk_UserTaskGoals_TaskID` FOREIGN KEY (`TaskID`) REFERENCES `Tasks` (`idTasks`) ON DELETE RESTRICT ON UPDATE RESTRICT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
