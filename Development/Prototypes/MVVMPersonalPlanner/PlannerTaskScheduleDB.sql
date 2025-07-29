SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;

DROP DATABASE IF EXISTS `PlannerTaskScheduleDB`;

CREATE DATABASE `PlannerTaskScheduleDB`;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserProfile`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserProfile` (
    `UserID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `LastName` VARCHAR(45) NOT NULL,
    `FirstName` VARCHAR(45) NOT NULL,
    `MiddleInitial` VARCHAR(45),
    `EmailAddress` VARCHAR(256),
    `LoginName` VARCHAR(45) NOT NULL,
    `HashedPassWord` TINYTEXT,
    `ScheduleDayStart` VARCHAR(45) NOT NULL,
    `ScheduleDayEnd` VARCHAR(45) NOT NULL,
    `IncludePriorityInSchedule` BOOLEAN DEFAULT TRUE,
    `IncludeMinorPriorityInSchedule` BOOLEAN DEFAULT TRUE,
    `UseLettersForMajorPriority` BOOLEAN DEFAULT TRUE,
    `SeparatePriorityWithDot` BOOLEAN DEFAULT FALSE,
    PRIMARY KEY (`UserID`, `LastName`, `LoginName`),
    UNIQUE INDEX `UserID_UNIQUE` (`UserID`),
    UNIQUE INDEX `FullName_UNIQUE` (`LastName`, `FirstName`, `MiddleInitial`),
    UNIQUE INDEX `LoginName_UNIQUE` (`LoginName` ASC)
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserGoals`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserGoals` (
    `idUserGoals` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `Description` TINYTEXT NOT NULL,
    `Priority` INT DEFAULT NULL,
    `ParentGoal` INT UNSIGNED DEFAULT NULL,
    PRIMARY KEY (`idUserGoals`, `UserID`),
    UNIQUE INDEX `idUserGoals_UNIQUE` (`idUserGoals`),
    CONSTRAINT `fk_UserGoals_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`UserID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX `fk_UserGoals_UserID_idx` (`UserID`)
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserNotes`;
CREATE TABLE IF NOT EXISTS `PlannerTaskScheduleDB`.`UserNotes` (
    `idUserNotes` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `NotationDateTime` DATETIME NOT NULL,
    `Content` VARCHAR(1024) NOT NULL,
    PRIMARY KEY (`idUserNotes`, `UserID`),
    UNIQUE INDEX `idUserNotes_UNIQUE` (`idUserNotes` ASC),
    INDEX `fk_UserNotes_UserID_idx` (`UserID` ASC),
    CONSTRAINT `fk_UserNotes_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `PlannerTaskScheduleDB`.`UserProfile` (`UserID`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);
    
-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`TaskStatusEnum`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`TaskStatusEnum` (
    `TaskIDtatusEnum` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskStatusEnumLable` VARCHAR(45) NOT NULL,
    PRIMARY KEY (`TaskIDtatusEnum`),
    UNIQUE INDEX `TaskIDtatusEnum_UNIQUE` (`TaskIDtatusEnum` ASC),
    UNIQUE INDEX `TaskStatusEnumLable_UNIQUE` (`TaskStatusEnumLable` ASC)
);

INSERT INTO PlannerTaskScheduleDB.TaskStatusEnum
    (TaskStatusEnumLable)
    VALUES
        ('Not Started'),
        ('On Hold'),
        ('Waiting for Dependency'),
        ('Work in Progress'),
        ('Complete');

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`Tasks`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`Tasks` (
    `TaskID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `CreatedBy` INT UNSIGNED NOT NULL,
    `AsignedTo` INT UNSIGNED NOT NULL,
    `Description` VARCHAR(256) NOT NULL,
    `ParentTask` INT UNSIGNED DEFAULT NULL,
    `Status` INT UNSIGNED DEFAULT NULL,
    `PercentageComplete` double NOT NULL,
    `CreatedOn` date NOT NULL,
    `RequiredDelivery` date NOT NULL,
    `ScheduledStart` date NOT NULL,
    `ActualStart` date DEFAULT NULL,
    `EstimatedCompletion` date DEFAULT NULL,
    `Completed` date DEFAULT NULL,
    `EstimatedEffortHours` INT UNSIGNED NOT NULL,
    `ActualEffortHours` double NOT NULL,
    `SchedulePriorityGroup` INT UNSIGNED NOT NULL,
    `PriorityInGroup` INT UNSIGNED NOT NULL,
    `Personal` BOOLEAN,
    `DependencyCount` INT UNSIGNED,
    PRIMARY KEY (`TaskID`, `CreatedBy`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID` ASC),
    INDEX `fk_Tasks_CreatedBy_idx` (`CreatedBy` ASC),
    INDEX `fk_Tasks_AsignedTo_idx` (`AsignedTo` ASC),
    INDEX `Description_idx` (`Description` ASC),
    CONSTRAINT `fk_Tasks_CreatedBy`
        FOREIGN KEY (`CreatedBy`)
        REFERENCES `UserProfile` (`UserID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT `fk_Tasks_AsignedTo`
        FOREIGN KEY (`AsignedTo`)
        REFERENCES `UserProfile` (`UserID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserTaskGoals`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserTaskGoals` (
    `UserID` INT UNSIGNED NOT NULL,
    `TaskID`  INT UNSIGNED NOT NULL,
    `TaskGoalList` VARCHAR(45) NOT NULL,
    PRIMARY KEY (`UserID`,`TaskID`),
    CONSTRAINT `fk_UserTaskGoals_AsignedTo`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`UserID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT `fk_UserTaskGoals_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`TaskID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`TaskDependencies`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`TaskDependencies` (
    `idTaskDependencies` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID`  INT UNSIGNED NOT NULL,
    `Dependency`  INT UNSIGNED NOT NULL,
    PRIMARY KEY (`idTaskDependencies`, `TaskID`),
    UNIQUE INDEX `idTaskDependencies_UNIQUE` (`idTaskDependencies` ASC),
    CONSTRAINT `fk_TaskDependencies_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`TaskID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS  `PlannerTaskScheduleDB`.`UserScheduleItemTypeEnum`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserScheduleItemTypeEnum` (
    `idUserScheduleItemTypeEnum` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserScheduleItemTypeEnumLabel` VARCHAR(45) NOT NULL,
    PRIMARY KEY (`idUserScheduleItemTypeEnum`),
    UNIQUE INDEX `idUserScheduleItemTypeEnum_UNIQUE` (`idUserScheduleItemTypeEnum` ASC),
    UNIQUE INDEX `UserScheduleItemTypeEnumLabel_UNIQUE` (`UserScheduleItemTypeEnumLabel` ASC)
);

INSERT INTO PlannerTaskScheduleDB.UserScheduleItemTypeEnum
    (UserScheduleItemTypeEnumLabel)
    VALUES
        ('Meeting'),
        ('Phone Call'),
        ('Task Execution'),
        ('Personal Appointment'),
        ('Personal Other');


-- --------------------------------------------------------

DROP TABLE IF EXISTS  `PlannerTaskScheduleDB`.`UserDaySchedule`;
CREATE TABLE IF NOT EXISTS `PlannerTaskScheduleDB`.`UserDaySchedule` (
    `idUserDaySchedule` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `DateOfSchedule` DATE NOT NULL,
    `StartOfDay` TIME NOT NULL,
    `EndOfDay` TIME NOT NULL,
    `DailyGoals` VARCHAR(45) NULL,
    PRIMARY KEY (`idUserDaySchedule`, `UserID`),
    UNIQUE INDEX `idUserDaySchedule_UNIQUE` (`idUserDaySchedule` ASC),
    INDEX `fk_UserDaySchedule_UserID_idx` (`UserID` ASC),
    CONSTRAINT `fk_UserDaySchedule_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `PlannerTaskScheduleDB`.`UserProfile` (`UserID`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS  `PlannerTaskScheduleDB`.`UserScheduleItem`;
CREATE TABLE IF NOT EXISTS `PlannerTaskScheduleDB`.`UserScheduleItem` (
    `idUserScheduleItem` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `StartDateTime` DATETIME NOT NULL,
    `EndDateTime` DATETIME NOT NULL,
    `ItemType` TINYINT NOT NULL,
    `Title` VARCHAR(128) NOT NULL,
    `Location` VARCHAR(45) DEFAULT NULL,
    PRIMARY KEY (`idUserScheduleItem`, `UserID`),
    UNIQUE INDEX `idUserScheduleItem_UNIQUE` (`idUserScheduleItem` ASC),
    INDEX `fk_UserScheduleItem_UserID_idx` (`UserID` ASC),
    CONSTRAINT `fk_UserScheduleItem_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `PlannerTaskScheduleDB`.`UserProfile` (`UserID`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);

-- -----------------------------------------------------
-- Stored Functions
-- -----------------------------------------------------

-- -----------------------------------------------------
-- function findTaskStatusEnumValueByLabel
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findTaskStatusEnumValueByLabel`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findTaskStatusEnumValueByLabel`(
    TaskStatusEnumLable VARCHAR(20)
) RETURNS INT
DETERMINISTIC
BEGIN
    
    SET @TaskStatusEnumKey = 0;
    
    SELECT TaskStatusEnum.TaskIDtatusEnum INTO @TaskStatusEnumKey
        FROM TaskStatusEnum
        WHERE TaskStatusEnum.TaskStatusEnumLable = TaskStatusEnumLable;
    IF @TaskStatusEnumKey IS NULL THEN
        SET @TaskStatusEnumKey = 0;
    END IF;

    RETURN @TaskStatusEnumKey;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function findTaskStatusEnumLabelByValue
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findTaskStatusEnumLabelByValue`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findTaskStatusEnumLabelByValue`(
    StatusKey INT
) RETURNS VARCHAR(20)
DETERMINISTIC
BEGIN
    
    SET @TaskStatusEnumLabel = ``;
    
    SELECT TaskStatusEnum.TaskStatusEnumLable INTO @TaskStatusEnumLabel
        FROM TaskStatusEnum
        WHERE TaskStatusEnum.TaskIDtatusEnum = TaskStatusEnumLable;
    IF @TasTaskStatusEnumLabelkStatusKey IS NULL THEN
        SET @TaskStatusEnumLabel = 'Not Started';
    END IF;

    RETURN @TaskStatusEnumLabel;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function findUserScheduleItemTypeEnumLabelByValue
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findUserScheduleItemTypeEnumLabelByValue`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findUserScheduleItemTypeEnumLabelByValue`(
    StatusKey INT
) RETURNS VARCHAR(20)
DETERMINISTIC
BEGIN
    
    SET @UserScheduleItemTypeEnumLabel = ``;
    
    SELECT UserScheduleItemTypeEnum.TaskStatusEnumLable INTO @UserScheduleItemTypeEnumLabel
        FROM UserScheduleItemTypeEnum
        WHERE UserScheduleItemTypeEnum.idUserScheduleItemTypeEnum = TaskStatusEnumLable;
    IF @TasUserScheduleItemTypeEnumLabelkStatusKey IS NULL THEN
        SET @UserScheduleItemTypeEnumLabel = 'Not Started';
    END IF;

    RETURN @UserScheduleItemTypeEnumLabel;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function findUserScheduleItemTypeEnumValueByLabel
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findUserScheduleItemTypeEnumValueByLabel`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findUserScheduleItemTypeEnumValueByLabel`(
    TaskStatusEnumLable VARCHAR(20)
) RETURNS INT
DETERMINISTIC
BEGIN
    
    SET @UserScheduleItemTypeEnumKey = 0;
    
    SELECT UserScheduleItemTypeEnum.idUserScheduleItemTypeEnum INTO @UserScheduleItemTypeEnumKey
        FROM UserScheduleItemTypeEnum
        WHERE UserScheduleItemTypeEnum.TaskStatusEnumLable = TaskStatusEnumLable;
    IF @UserScheduleItemTypeEnumKey IS NULL THEN
        SET @UserScheduleItemTypeEnumKey = 0;
    END IF;

    RETURN @UserScheduleItemTypeEnumKey;
    
END$$

DELIMITER ;


-- -----------------------------------------------------
-- function findUserIDKeyByLoginName
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findUserIDKeyByLoginName`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findUserIDKeyByLoginName`(
    LoginName VARCHAR(45)
) RETURNS INT
DETERMINISTIC
BEGIN

    SET @UserIDKey = 0;

    SELECT UserLoginAndPassword.UserID INTO @UserIDKey
        FROM UserLoginAndPassword
        WHERE UserLoginAndPassword.LoginName = LoginName;
    IF @UserIDKey IS NULL THEN
        SET @UserIDKey = 0;
    END IF;

    RETURN @UserIDKey;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function findUserIDKeyByFullName
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findUserIDKeyByFullName`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findUserIDKeyByFullName`(
    LastName VARCHAR(45),
    FirstName TINYTEXT,
    MiddleInitial TINYTEXT
) RETURNS INT
DETERMINISTIC
BEGIN

    SET @UserIDKey = 0;

    SELECT UserProfile.UserID INTO @UserIDKey
        FROM UserProfile
        WHERE UserProfile.LastName = LastName AND
            UserProfile.FirstName = FirstName AND
            UserProfile.MiddleInitial = MiddleInitial;
    IF @UserIDKey IS NULL THEN
        SET @UserIDKey = 0;
    END IF;

    RETURN @UserIDKey;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function isValidUserLoginAndPassword
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `isValidUserLoginAndPassword`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `isValidUserLoginAndPassword`
(
    LoginName VARCHAR(45),
    HashedPassWord TINYTEXT
)
RETURNS TINYINT
DETERMINISTIC
BEGIN

    SET @UserIDKey = 0;
    SET @isValid = 1;

    SELECT UserLoginAndPassword.UserID INTO @UserIDKey
        FROM UserLoginAndPassword
        WHERE UserLoginAndPassword.LoginName = LoginName AND
            UserLoginAndPassword.HashedPassWord = HashedPassWord;

    IF @UserIDKey IS NULL THEN
        SET @isValid = 0;
    END IF;

    RETURN @isValid;

END$$

DELIMITER ;

COMMIT;

