SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;

DROP DATABASE IF EXISTS `testPTSDB`;

CREATE DATABASE `testPTSDB`;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `testPTSDB`.`OrganizationProfile`;
CREATE TABLE IF NOT EXISTS  `testPTSDB`.`OrganizationProfile` (
	`OrganizationID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `Organization_Name` VARCHAR(256) NOT NULL,
    `EmailAddress` VARCHAR(256) NOT NULL,
    `PhoneNumber` VARCHAR(32) NOT NULL,
    `PrimaryContactUser` INT UNSIGNED,
    `SecondaryContactUser` INT UNSIGNED,
    `Street_Address` VARCHAR(256),
    `City_Address` VARCHAR(64),
    `Postal_Code` VARCHAR(32),
    `State_or_Province` VARCHAR(256),
    `Nation` VARCHAR(64),
    `CreatedTS` DATETIME NOT NULL,
    `LastUpdateTS` DATETIME NOT NULL,
    `Hidden` TINYINT,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (`OrganizationID`),
    UNIQUE INDEX `OrgName_idx` (`Organization_Name` ASC),
    INDEX `fk_OrganizationProfile_PrimaryContact_idx` (`PrimaryContactUser` ASC)
);
-- --------------------------------------------------------

DROP TABLE IF EXISTS `testPTSDB`.`UserProfile`;
CREATE TABLE IF NOT EXISTS  `testPTSDB`.`UserProfile` (
    `UserID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `Organization_ID` INT UNSIGNED,
    `LastName` VARCHAR(45) NOT NULL,
    `FirstName` VARCHAR(45) NOT NULL,
    `MiddleInitial` VARCHAR(45),
    `EmailAddress` VARCHAR(256),
    `LoginName` VARCHAR(45) NOT NULL,
    `HashedPassWord` VARCHAR(45) NOT NULL,
    `Preferences` MEDIUMTEXT NOT NULL,
    `UserAdded` DATETIME NOT NULL,
    `LastLogin` DATETIME,
    `Hidden` TINYINT,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (`UserID`, `LastName`, `LoginName`),
    UNIQUE INDEX `UP_UserID_UNIQUE` (`UserID` ASC),
    UNIQUE INDEX `UP_FullName_UNIQUE` (`LastName`, `FirstName`, `MiddleInitial`),
    UNIQUE INDEX `UP_LoginName_UNIQUE` (`LoginName` ASC),
    UNIQUE INDEX `UP_Email_UNIQUE` (`EmailAddress` ASC),
    INDEX `UP_LastLogin` (`LastLogin` DESC),
    CONSTRAINT `fk_UserProfile_OrgID`
        FOREIGN KEY (`Organization_ID`)
        REFERENCES `OrganizationProfile` (`OrganizationID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX `fk_UserProfile_OrgID_idx` (`Organization_ID`)
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `testPTSDB`.`UserGoals`;
CREATE TABLE IF NOT EXISTS  `testPTSDB`.`UserGoals` (
    `idUserGoals` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `Description` VARCHAR(1024) NOT NULL,
    `Priority` INT DEFAULT NULL,
    `ParentGoal` INT UNSIGNED DEFAULT NULL,
    `CreationTS` DATETIME NOT NULL,
    `LastUpdateTS` DATETIME NOT NULL,
    `Hidden` TINYINT,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (`idUserGoals`, `UserID`),
    UNIQUE INDEX `idUserGoals_UNIQUE` (`idUserGoals` ASC),
    INDEX `UG_Description_idx` (`Description` ASC),
    INDEX `UG_CreationTS_idx` (`CreationTS` DESC),
    INDEX `UG_LastUpdateTS_idx` (`LastUpdateTS` DESC),
    CONSTRAINT `fk_UserGoals_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`UserID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX `fk_UserGoals_UserID_idx` (`UserID`)
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `testPTSDB`.`UserNotes`;
CREATE TABLE IF NOT EXISTS `testPTSDB`.`UserNotes` (
    `idUserNotes` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `NotationDateTime` DATETIME NOT NULL,
    `Content` VARCHAR(1024) NOT NULL,
    `LastUpdate` DATETIME NOT NULL,
    `Hidden` TINYINT,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (`idUserNotes`, `UserID`),
    INDEX `NotationDateTime` (`NotationDateTime` DESC),
    INDEX `UserNotesLastUpdate` (`LastUpdate` DESC),
    UNIQUE INDEX `idUserNotes_UNIQUE` (`idUserNotes` ASC),
    INDEX `UN_Content_idx` (`Content` ASC),
    INDEX `fk_UserNotes_UserID_idx` (`UserID` ASC),
    CONSTRAINT `fk_UserNotes_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `testPTSDB`.`UserProfile` (`UserID`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);
    
-- --------------------------------------------------------

DROP TABLE IF EXISTS `testPTSDB`.`Tasks`;
CREATE TABLE IF NOT EXISTS  `testPTSDB`.`Tasks` (
    `TaskID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `CreatedBy` INT UNSIGNED NOT NULL,
    `AsignedTo` INT UNSIGNED NOT NULL,
    `Description` VARCHAR(256) NOT NULL,
    `ParentTask` INT UNSIGNED DEFAULT NULL,
    `Status` INT UNSIGNED DEFAULT NULL,
    `PercentageComplete` double NOT NULL,
    `CreatedOn` DATETIME NOT NULL,
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
    `Dependencies` MEDIUMTEXT,
    `LastUpdateTS` DATETIME NOT NULL,
    `Hidden` TINYINT,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (`TaskID`, `CreatedBy`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID` ASC),
    INDEX `fk_Tasks_CreatedBy_idx` (`CreatedBy` ASC),
    INDEX `fk_Tasks_AsignedTo_idx` (`AsignedTo` ASC),
    INDEX `Task_Description_idx` (`Description` ASC),
    INDEX `Task_LastUpdateTS_idx` (`LastUpdateTS` DESC),
    INDEX `Task_SchedulePriorityGroup_idx` (`SchedulePriorityGroup` ASC),
    INDEX `Task_PriorityInGroup_idx` (`PriorityInGroup` ASC),
    INDEX `Task_Status_idx` (`Status` ASC),
    INDEX `Task_RequiredDelivery_idx` (`RequiredDelivery` ASC),
    INDEX `Task_ScheduledStart_idx` (`ScheduledStart` ASC),
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

DROP TABLE IF EXISTS `testPTSDB`.`UserTaskGoals`;
CREATE TABLE IF NOT EXISTS  `testPTSDB`.`UserTaskGoals` (
    `UserID` INT UNSIGNED NOT NULL,
    `TaskID`  INT UNSIGNED NOT NULL,
    `TaskGoalList` VARCHAR(45) NOT NULL,
    `CreationTS` DATETIME NOT NULL,
    `LastUpdateTS` DATETIME NOT NULL,
    `Hidden` TINYINT,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (`UserID`,`TaskID`),
    INDEX `UTG_Task_idx` (`TaskID` ASC),
    INDEX `UTG_CreationTS_idx` (`CreationTS` DESC),
    INDEX `UTG_LastUpdateTS_idx` (`LastUpdateTS` DESC),
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

DROP TABLE IF EXISTS  `testPTSDB`.`UserScheduleItem`;
CREATE TABLE IF NOT EXISTS `testPTSDB`.`UserScheduleItem` (
    `idUserScheduleItem` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `StartDateTime` DATETIME NOT NULL,
    `EndDateTime` DATETIME NOT NULL,
    `Title` VARCHAR(128) NOT NULL,
    `Personal` TINYINT NOT NULL,
    `Location` VARCHAR(128) DEFAULT NULL,
    `CreatedTS` DATETIME NOT NULL,
    `LastUpdateTS` DATETIME NOT NULL,
    `Hidden` TINYINT,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (`idUserScheduleItem`, `UserID`),
    UNIQUE INDEX `idUserScheduleItem_UNIQUE` (`idUserScheduleItem` ASC),
    INDEX `ScheduleItem_Title_idx` (`Title` ASC),
    INDEX `ScheduleItem_StartDateTime_idx` (`StartDateTime` DESC),
    INDEX `ScheduleItem_CreatedTS_idx` (`CreatedTS` DESC),
    INDEX `ScheduleItem_LastUpdateTS_idx` (`LastUpdateTS` DESC),
    INDEX `fk_UserScheduleItem_UserID_idx` (`UserID` ASC),
    INDEX `ScheduleItem_Location_idx` (`Location` ASC),
    CONSTRAINT `fk_UserScheduleItem_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `testPTSDB`.`UserProfile` (`UserID`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);

-- -----------------------------------------------------
-- Stored Functions
-- -----------------------------------------------------

-- -----------------------------------------------------
-- function findUserIDKeyByLoginName
-- -----------------------------------------------------

USE `testPTSDB`;
DROP function IF EXISTS `testPTSDB`.`findUserIDKeyByLoginName`;

DELIMITER $$
USE `testPTSDB`$$
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

USE `testPTSDB`;
DROP function IF EXISTS `testPTSDB`.`findUserIDKeyByFullName`;

DELIMITER $$
USE `testPTSDB`$$
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

USE `testPTSDB`;
DROP function IF EXISTS `isValidUserLoginAndPassword`;

DELIMITER $$
USE `testPTSDB`$$
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

-- -----------------------------------------------------
-- Stored Procedures
-- -----------------------------------------------------

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`ScheduleItemContentSelectionList`;

CREATE PROCEDURE `testPTSDB`.`ScheduleItemContentSelectionList`
(
    IN Content VARCHAR(128),
    IN IDUser INT
)

BEGIN

    SELECT DISTINCT UserScheduleItem.Title FROM UserScheduleItem
    WHERE
        UserScheduleItem.UserID = IDUser AND UserScheduleItem.Title LIKE CONCAT('%', Content, '%')
        ORDER BY UserScheduleItem.Title ASC;

END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`EventTitlesForCompleter`;

CREATE PROCEDURE `testPTSDB`.`EventTitlesForCompleter`
(
    IN IDUser INT
)

BEGIN

    SELECT DISTINCT UserScheduleItem.Title FROM UserScheduleItem
    WHERE
        UserScheduleItem.UserID = IDUser
        ORDER BY UserScheduleItem.Title ASC;

END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`EventLocationsForCompleter`;

CREATE PROCEDURE `testPTSDB`.`EventLocationsForCompleter`
(
    IN IDUser INT
)

BEGIN

    SELECT DISTINCT UserScheduleItem.Location FROM UserScheduleItem
    WHERE
        UserScheduleItem.UserID = IDUser
        ORDER BY UserScheduleItem.Location ASC;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- Stored procedures to hide a record the user wants to delete
-- -----------------------------------------------------

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`HideOrganization`;

CREATE PROCEDURE `testPTSDB`.`HideOrganization`
(
    IN OrgID INT 
)

BEGIN

    UPDATE OrganizationProfile
        SET Hidden = 1
        WHERE Organization_ID = OrgID;

END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`HideUser`;

CREATE PROCEDURE `testPTSDB`.`HideUser`
(
    IN IDUser INT 
)

BEGIN

    UPDATE UserProfile
        SET Hidden = 1
        WHERE UserID = IDUser;

END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`HideGoal`;

CREATE PROCEDURE `testPTSDB`.`HideGoal`
(
    IN IDUser INT,
    IN GoalID INT
)

BEGIN

    UPDATE UserGoals
        SET Hidden = 1, LastUpdateTS = NOW()
        WHERE UserID = IDUser AND idUserGoals = GoalID;

END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`HideNote`;

CREATE PROCEDURE `testPTSDB`.`HideNote`
(
    IN IDUser INT,
    IN NoteID INT
)

BEGIN

    UPDATE UserNotes
        SET Hidden = 1, LastUpdate = NOW()
        WHERE UserID = IDUser AND idUserNotes = NoteID;

END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`HideTask`;

CREATE PROCEDURE `testPTSDB`.`HideTask`
(
    IN IDUser INT,
    IN IDTask INT
)

BEGIN

    UPDATE Tasks
        SET Hidden = 1, LastUpdateTS = NOW()
        WHERE CreatedBy = IDUser AND TaskID = IDTask;

END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`HideScheduleItem`;

CREATE PROCEDURE `testPTSDB`.`HideScheduleItem`
(
    IN IDUser INT,
    IN ScheduleItemID INT
)

BEGIN

    UPDATE UserScheduleItem
        SET Hidden = 1, LastUpdateTS = NOW()
        WHERE UserID = IDUser AND idUserScheduleItem = ScheduleItemID;

END$$

DELIMITER ;

USE `testPTSDB`;
DELIMITER $$

DROP PROCEDURE IF EXISTS `AddOrganization`$$

CREATE PROCEDURE `AddOrganization`
(
    IN `orgName` VARCHAR(256),
    IN `email` VARCHAR(256),
    IN `phoneNo` VARCHAR(32),
    IN `primaryContact` INT UNSIGNED,
    IN `secondaryContact` INT UNSIGNED,
    IN `streetAddress` VARCHAR(256),
    IN `city` VARCHAR(64),
    IN `postalCode` VARCHAR(32),
    IN `stateOrProvince` VARCHAR(64),
    IN `nation` VARCHAR(64)
)
BEGIN

    INSERT INTO OrganizationProfile (
        Organization_Name,
        EmailAddress,
        PhoneNumber,
        PrimaryContactUser,
        SecondaryContactUser,
        Street_Address,
        City_Address,
        State_or_Province,
        Postal_Code,
        Nation,
        CreatedTS,
        LastUpdateTS,
        Hidden
        )
    VALUES (
        orgName, email, phoneNo, primaryContact, secondaryContact, streetAddress, city, stateOrProvince, postalCode, nation, NOW(), NOW(), 0
    ) RETURNING OrganizationID;

END$$

DELIMITER ;

DELIMITER $$

DROP PROCEDURE IF EXISTS `AddUser`$$

CREATE PROCEDURE `AddUser`
(
    IN `OrgId` INT UNSIGNED,
    IN `lastName` VARCHAR(45),
    IN `firstName` VARCHAR(45),
    IN `middleInitial` VARCHAR(45),
    IN `email` VARCHAR(256),
    IN `username` VARCHAR(45),
    IN `password` VARCHAR(45),
    IN `preferrences` MEDIUMTEXT
)
BEGIN

	INSERT INTO UserProfile
    	(
            Organization_ID,
            LastName,
            FirstName,
            MiddleInitial,
            EmailAddress,
            LoginName,
            HashedPassWord,
            UserAdded,
            Preferences,
            Hidden
        )
        VALUES
        (
            OrgId,
            lastName,
            firstName,
            middleInitial,
            email,
            username,
            password,
            NOW(),
            preferrences,
            0
        )
        RETURNING UserID;

END$$

DELIMITER ;

DELIMITER $$

DROP PROCEDURE IF EXISTS `UpdateUserAllFields`$$

CREATE PROCEDURE `UpdateUserAllFields`
(
    IN `userId` INT UNSIGNED,
    IN `OrgId` INT UNSIGNED,
    IN `lastName` VARCHAR(45),
    IN `firstName` VARCHAR(45),
    IN `middleInitial` VARCHAR(45),
    IN `email` VARCHAR(256),
    IN `username` VARCHAR(45),
    IN `password` VARCHAR(45),
    IN `preferrences` MEDIUMTEXT,
    IN `lastLogin` DATETIME
)

BEGIN

    UPDATE UserProfile SET
        UserProfile.Organization_ID = OrgId,
        UserProfile.LastName = lastName,
        UserProfile.FirstName = firstName,
        UserProfile.MiddleInitial = middleInitial,
        UserProfile.EmailAddress = email,
        UserProfile.LoginName = username,
        UserProfile.HashedPassWord = password,
        UserProfile.Preferences = preferrences,
        UserProfile.LastLogin = lastLogin
    WHERE UserProfile.UserID = userId;

END$$

DELIMITER ;

DELIMITER $$

DROP PROCEDURE IF EXISTS `AddUserGoal`$$

CREATE PROCEDURE `AddUserGoal`
(
    IN `userId` INT UNSIGNED,
    IN `description` VARCHAR(1024),
    IN `priority` INT ZEROFILL,
    IN `parentGoalId` INT UNSIGNED ZEROFILL
)

BEGIN

    INSERT INTO UserGoals
        (
            UserID,
            Description,
            CreationTS,
            LastUpdateTS,
            Priority,
            ParentGoal,
            Hidden
        )
        VALUES
        (
            userId,
            description,
            NOW(),
            NOW(),
            priority,
            parentGoalId,
            0
        )
        RETURNING idUserGoals;

END$$

DELIMITER ;

DELIMITER $$

DROP PROCEDURE IF EXISTS `AddTask`$$

CREATE PROCEDURE `AddTask`
(
    IN `creatorID` INT UNSIGNED,
    IN `assignedID` INT UNSIGNED,
    IN `description` VARCHAR(256),
    IN `parentTaskID` INT UNSIGNED,
    IN `taskStatus` INT UNSIGNED,
    IN `dueDate` DATE,
    IN `planStart` DATE,
    IN `startDate` DATE,
    IN `expectedDate` DATE,
    IN `completedDate` DATE,
    IN `estimatedEffort` INT UNSIGNED,
    IN `effortToDate` DOUBLE,
    IN `priorityCategory` INT UNSIGNED,
    IN `priority` INT UNSIGNED,
    IN `isPersonal` TINYINT,
    IN `dependencyCount` INT UNSIGNED,
    IN `dependencies` MEDIUMTEXT
)
BEGIN

	INSERT INTO Tasks
    (
        CreatedBy,
        AsignedTo,
        Description,
        ParentTask,
        Status,
        PercentageComplete,
        CreatedOn,
        RequiredDelivery,
        ScheduledStart,
        ActualStart,
        EstimatedCompletion,
        Completed,
        EstimatedEffortHours,
        ActualEffortHours,
        SchedulePriorityGroup,
        PriorityInGroup,
        Personal,
        DependencyCount,
        Dependencies,
        LastUpdateTS,
        Hidden
    )
    VALUES
    (
        creatorID,
        assignedID,
        description,
        parentTaskID,
        taskStatus,
        0.0,
        NOW(),
        dueDate,
        planStart,
        startDate,
        expectedDate,
        completedDate,
        estimatedEffort,
        effortToDate,
        priorityCategory,
        priority,
        isPersonal,
        dependencyCount,
        dependencies,
        NOW(),
        0
    )
    RETURNING TaskID;

END$$
DELIMITER ;

DELIMITER $$

DROP PROCEDURE IF EXISTS `AddUserNote`$$

CREATE PROCEDURE `AddUserNote`
(
    IN `userId` INT UNSIGNED,
    IN `content` VARCHAR(1024)
)

BEGIN

	INSERT INTO UserNotes(UserID, NotationDateTime, Content, LastUpdate, Hidden)
        VALUES (userId, NOW(), content, NOW(), 0)
        RETURNING idUserNotes;

END$$

DELIMITER ;

DELIMITER $$

DROP PROCEDURE IF EXISTS `AddScheduleEvent`$$

CREATE PROCEDURE `AddScheduleEvent`
(
    IN `userId` INT UNSIGNED,
    IN `eventStart` DATETIME,
    IN `eventEnd` DATETIME,
    IN `eventTitle` VARCHAR(128),
    IN `isPersonal` TINYINT,
    IN `location` VARCHAR(128)
)

BEGIN

INSERT INTO UserScheduleItem
	(
        UserID, StartDateTime, EndDateTime, Title, Personal, Location, CreatedTS, LastUpdateTS, Hidden
    )
    VALUES
    (
        userId,
        eventStart,
        eventEnd,
        eventTitle,
        isPersonal,
        location,
        NOW(),
        NOW(),
        0
    )
    RETURNING idUserScheduleItem;
    
END$$
DELIMITER ;

COMMIT;


