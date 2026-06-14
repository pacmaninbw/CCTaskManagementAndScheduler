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
    `Hidden` TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (`OrganizationID`),
    UNIQUE INDEX `OrgName_idx` (`Organization_Name` ASC),
    INDEX `fk_OrganizationProfile_PrimaryContact_idx` (`PrimaryContactUser` ASC)
);

-- --------------------------------------------------------
-- Stored Procedures for OrganizationProfile
-- --------------------------------------------------------

DELIMITER $$
USE `testPTSDB`;

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
        orgName, email, phoneNo, primaryContact, secondaryContact, streetAddress, city, stateOrProvince, postalCode, nation, UTC_TIMESTAMP(), UTC_TIMESTAMP(), 0
    ) RETURNING OrganizationID;

END$$

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
    `Hidden` TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
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
-- Stored Procedures for UserProfile
-- --------------------------------------------------------

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
            UTC_TIMESTAMP(),
            preferrences,
            0
        )
        RETURNING UserID;

END$$

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

DROP PROCEDURE IF EXISTS `GetAllUsers`$$

CREATE PROCEDURE `GetAllUsers`
()

BEGIN

    SELECT * FROM UserProfile;

END$$

DROP PROCEDURE IF EXISTS `GetUserByID`$$

CREATE PROCEDURE `GetUserByID`
(
    IN `userId` INT UNSIGNED
)

BEGIN

    SELECT * FROM UserProfile WHERE UserProfile.UserID = userId;

END$$

DROP PROCEDURE IF EXISTS `GetUserByLoginName`$$

CREATE PROCEDURE `GetUserByLoginName`
(
    IN `username` VARCHAR(45)
)

BEGIN

    SELECT * FROM UserProfile WHERE UserProfile.LoginName = username;

END$$

DROP PROCEDURE IF EXISTS `GetUserByEmail`$$

CREATE PROCEDURE `GetUserByEmail`
(
    IN `email` VARCHAR(256)
)

BEGIN

    SELECT * FROM UserProfile WHERE UserProfile.EmailAddress = email;

END$$

DROP PROCEDURE IF EXISTS `GetUserByLoginAndPassword`$$

CREATE PROCEDURE `GetUserByLoginAndPassword`
(
    IN `username` VARCHAR(45),
    IN `password` VARCHAR(45)
)

BEGIN

    SELECT * FROM UserProfile WHERE UserProfile.LoginName = username  AND UserProfile.HashedPassWord = password;

END$$

DROP PROCEDURE IF EXISTS `GetUserByFullName`$$

CREATE PROCEDURE `GetUserByFullName`
(
    IN `lastName` VARCHAR(45),
    IN `firstName` VARCHAR(45),
    IN `middleInitial` VARCHAR(45)
)

BEGIN

    SELECT * FROM UserProfile WHERE UserProfile.LastName = lastName AND UserProfile.FirstName = firstName AND UserProfile.MiddleInitial = middleInitial;

END$$

-- -----------------------------------------------------
-- Stored Functions
-- -----------------------------------------------------

-- -----------------------------------------------------
-- function findUserIDKeyByLoginName
-- -----------------------------------------------------

USE `testPTSDB`;
DROP function IF EXISTS `testPTSDB`.`findUserIDKeyByLoginName`;

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


-- -----------------------------------------------------
-- function findUserIDKeyByFullName
-- -----------------------------------------------------

DROP function IF EXISTS `testPTSDB`.`findUserIDKeyByFullName`;

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


-- -----------------------------------------------------
-- function isValidUserLoginAndPassword
-- -----------------------------------------------------

DROP function IF EXISTS `isValidUserLoginAndPassword`;

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
    `Hidden` TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
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
-- Stored Procedures for UserGoals
-- --------------------------------------------------------

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
        SET Hidden = 1, LastUpdateTS = UTC_TIMESTAMP()
        WHERE UserID = IDUser AND idUserGoals = GoalID;

END$$

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
            UTC_TIMESTAMP(),
            UTC_TIMESTAMP(),
            priority,
            parentGoalId,
            0
        )
        RETURNING idUserGoals;

END$$

DROP PROCEDURE IF EXISTS `UpdateUserGoalAllFields`$$

CREATE PROCEDURE `UpdateUserGoalAllFields`
(
    IN `userIdIn` INT UNSIGNED,
    IN `goalId` INT UNSIGNED,
    IN `description` VARCHAR(1024),
    IN `priority` INT UNSIGNED,
    IN `parentGoalID` INT UNSIGNED
)
BEGIN

    UPDATE UserGoals SET
        UserGoals.Description = description,
        UserGoals.LastUpdateTS = UTC_TIMESTAMP(),
        UserGoals.Priority = priority,
        UserGoals.ParentGoal = parentGoalID
    WHERE UserGoals.UserID = userIdIn AND UserGoals.idUserGoals = goalId;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetGoalById`;

CREATE PROCEDURE `testPTSDB`.`GetGoalById`
(
    IN `goalId` INT UNSIGNED
)

BEGIN

    SELECT * FROM UserGoals WHERE UserGoals.idUserGoals = goalId;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`FindGoalByUserIdAndExactDescription`;

CREATE PROCEDURE `testPTSDB`.`FindGoalByUserIdAndExactDescription`
(
    IN `userId` INT UNSIGNED,
    IN `fullDescription` VARCHAR(1024)
)

BEGIN

    SELECT * FROM UserGoals
    WHERE UserGoals.UserID = userId
    AND UserGoals.Description = fullDescription
        AND UserGoals.Hidden <> 1;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetAllGoalsForUser`;

CREATE PROCEDURE `testPTSDB`.`GetAllGoalsForUser`
(
    IN `userId` INT UNSIGNED
)

BEGIN

    SELECT * FROM UserGoals
    WHERE UserGoals.UserID = userId
        AND UserGoals.Hidden <> 1;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetAllChildGoalsFromParent`;

CREATE PROCEDURE `testPTSDB`.`GetAllChildGoalsFromParent`
(
    IN `userId` INT UNSIGNED,
    IN `parentGoalId` INT UNSIGNED
)

BEGIN

    SELECT * FROM UserGoals
    WHERE UserGoals.UserID = userId
        AND UserGoals.ParentGoal = parentGoalId
        AND UserGoals.Hidden <> 1;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`FindGoalsByUserIdAndSimilarDescription`;

CREATE PROCEDURE `testPTSDB`.`FindGoalsByUserIdAndSimilarDescription`
(
    IN `userId` INT UNSIGNED,
    IN `partialDescription` VARCHAR(1024)
)

BEGIN

    SELECT * FROM UserGoals
    WHERE UserGoals.UserID = userId AND UserGoals.Description LIKE CONCAT('%', partialDescription, '%')
        AND UserGoals.Hidden <> 1;

END$$

DELIMITER ;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `testPTSDB`.`UserNotes`;
CREATE TABLE IF NOT EXISTS `testPTSDB`.`UserNotes` (
    `idUserNotes` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `NotationDateTime` DATETIME NOT NULL,
    `Content` VARCHAR(1024) NOT NULL,
    `LastUpdate` DATETIME NOT NULL,
    `Hidden` TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
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
-- Stored Procedures for UserNotes
-- --------------------------------------------------------

USE `testPTSDB`;

DELIMITER $$
DROP PROCEDURE IF EXISTS `testPTSDB`.`HideNote`;

CREATE PROCEDURE `testPTSDB`.`HideNote`
(
    IN IDUser INT,
    IN NoteID INT
)

BEGIN

    UPDATE UserNotes
        SET Hidden = 1, LastUpdate = UTC_TIMESTAMP()
        WHERE UserID = IDUser AND idUserNotes = NoteID;

END$$

DROP PROCEDURE IF EXISTS `AddUserNote`$$

CREATE PROCEDURE `AddUserNote`
(
    IN `userId` INT UNSIGNED,
    IN `content` VARCHAR(1024)
)

BEGIN

    INSERT INTO UserNotes(UserID, NotationDateTime, Content, LastUpdate, Hidden)
        VALUES (userId, UTC_TIMESTAMP(), content, UTC_TIMESTAMP(), 0)
        RETURNING idUserNotes;

END$$

DROP PROCEDURE IF EXISTS `UpdateNoteAllFields`$$

CREATE PROCEDURE `UpdateNoteAllFields`
(
    IN `userId` INT UNSIGNED,
    IN `noteId` INT UNSIGNED,
    IN `content` VARCHAR(1024)
)

BEGIN

    UPDATE UserNotes SET
        UserNotes.Content = content,
        UserNotes.LastUpdate = UTC_TIMESTAMP()
    WHERE UserNotes.idUserNotes = noteId AND UserNotes.UserID = userId;
    
END$$

DROP PROCEDURE IF EXISTS `GetNoteByID`$$

CREATE PROCEDURE `GetNoteByID`
(
    IN `noteId` INT UNSIGNED
)

BEGIN
 
    SELECT * FROM UserNotes  WHERE UserNotes.idUserNotes = noteId;
 
END$$
 
DROP PROCEDURE IF EXISTS `GetAllNotesForUser`$$
 
CREATE PROCEDURE `GetAllNotesForUser` (IN `userId` INT UNSIGNED)   BEGIN
  
    SELECT * FROM UserNotes WHERE UserNotes.UserID = userId;

END$$
 
DROP PROCEDURE IF EXISTS `GetAllUndeletedNotesForUser`$$

CREATE PROCEDURE `GetAllUndeletedNotesForUser`
(
    IN `userId` INT UNSIGNED
)

BEGIN

    SELECT * FROM UserNotes
    WHERE UserNotes.UserID = userId
        AND UserNotes.Hidden <> 1;
 
END$$

DROP PROCEDURE IF EXISTS `GetNotesForUserSimlarToContent`$$
 
CREATE PROCEDURE `GetNotesForUserSimlarToContent`
(
    IN `userId` INT UNSIGNED,
    IN `likeContent` VARCHAR(1024)
)
BEGIN
 
    SELECT * FROM UserNotes 
    WHERE UserNotes.UserID = userId
        AND UserNotes.Content LIKE CONCAT('%', likeContent, '%')
        AND UserNotes.Hidden <> 1;
    
END$$    
 
DROP PROCEDURE IF EXISTS `GetAllNotesForUserCreatedInDatgeRange`$$

CREATE PROCEDURE `GetAllNotesForUserCreatedInDatgeRange`
(
    IN `UserID` INT UNSIGNED,
    IN `timePeriodStart` DATE,
    IN `timePeriodEnd` DATE
) 
BEGIN

    SELECT * FROM UserNotes
    WHERE UserNotes.UserID = 4
        AND UserNotes.NotationDateTime >= timePeriodStart
        AND UserNotes.NotationDateTime <= timePeriodEnd
        AND UserNotes.Hidden <> 1;
 
END$$
 
DROP PROCEDURE IF EXISTS `GetAllNotesForUserEditedInDatgeRange`$$

CREATE PROCEDURE `GetAllNotesForUserEditedInDatgeRange`
(
    IN `UserID` INT UNSIGNED,
    IN `timePeriodStart` DATE,
    IN `timePeriodEnd` DATE
) 
BEGIN

    SELECT * FROM UserNotes
    WHERE UserNotes.UserID = 4
        AND UserNotes.LastUpdate >= timePeriodStart
        AND UserNotes.LastUpdate <= timePeriodEnd
        AND UserNotes.Hidden <> 1;
 
END$$
 
DROP PROCEDURE IF EXISTS `GetDashboardNoteTable`$$

CREATE PROCEDURE `GetDashboardNoteTable` 
(
    IN `userId` INT UNSIGNED,
    IN `startDay` DATETIME,
    IN `endDay` DATETIME
)
BEGIN

    SELECT * FROM UserNotes
    WHERE UserNotes.UserID = userId
        AND UserNotes.NotationDateTime >= startDay
        AND UserNotes.NotationDateTime <= endDay
        AND UserNotes.Hidden <> 1
    ORDER BY UserNotes.NotationDateTime ASC;

END$$

DELIMITER ;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `testPTSDB`.`Tasks`;
CREATE TABLE IF NOT EXISTS  `testPTSDB`.`Tasks` (
    `TaskID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `CreatedBy` INT UNSIGNED NOT NULL,
    `AsignedTo` INT UNSIGNED NOT NULL,
    `Description` VARCHAR(256) NOT NULL,
    `ParentTask` INT UNSIGNED DEFAULT NULL,
    `Status` INT UNSIGNED DEFAULT NULL,
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
    `Hidden` TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
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
-- Stored Procedures for Tasks
-- --------------------------------------------------------

USE `testPTSDB`;

DELIMITER $$
DROP PROCEDURE IF EXISTS `testPTSDB`.`HideTask`;

CREATE PROCEDURE `testPTSDB`.`HideTask`
(
    IN IDUser INT,
    IN IDTask INT
)

BEGIN

    UPDATE Tasks
        SET Hidden = 1, LastUpdateTS = UTC_TIMESTAMP()
        WHERE CreatedBy = IDUser AND TaskID = IDTask;

END$$

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
        UTC_TIMESTAMP(),
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
        UTC_TIMESTAMP(),
        0
    )
    RETURNING TaskID;

END$$

DROP PROCEDURE IF EXISTS `UpdateTaskAllFields`$$

CREATE PROCEDURE `UpdateTaskAllFields`
(
    IN `primaryKeyValue` INT UNSIGNED,
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

    UPDATE Tasks SET
        CreatedBy = creatorID,
        AsignedTo = assignedID,
        Description = description,
        ParentTask = parentTaskID,
        Status = taskStatus,
        RequiredDelivery = dueDate,
        ScheduledStart = planStart,
        ActualStart = startDate,
        EstimatedCompletion = expectedDate,
        Completed = completedDate,
        EstimatedEffortHours = estimatedEffort,
        ActualEffortHours = effortToDate,
        SchedulePriorityGroup = priorityCategory,
        PriorityInGroup = priority,
        Personal = isPersonal,
        DependencyCount = dependencyCount,
        Dependencies = dependencies,
        LastUpdateTS = UTC_TIMESTAMP(),
        Hidden = 0
    WHERE TaskID = primaryKeyValue;
    
END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetTaskByTaskID`;

CREATE PROCEDURE `testPTSDB`.`GetTaskByTaskID`
(
    IN IDTask INT
)

BEGIN

    SELECT * FROM Tasks WHERE Tasks.TaskID = IDTask;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetTaskByDescriptionAndAssignedUser`;

CREATE PROCEDURE `testPTSDB`.`GetTaskByDescriptionAndAssignedUser`
(
    IN `assignedID` INT UNSIGNED,
    IN `description` VARCHAR(256)
)

BEGIN

    SELECT * FROM Tasks
    WHERE Tasks.Description = description
        AND Tasks.AsignedTo = assignedID
        AND Tasks.Hidden <> 1;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetActiveTasksForAssignedUser`;

CREATE PROCEDURE `testPTSDB`.`GetActiveTasksForAssignedUser`
(
    IN `assignedID` INT UNSIGNED
)

BEGIN

    SELECT * FROM Tasks
    WHERE Tasks.AsignedTo = assignedID
        AND Tasks.Completed IS NULL
        AND (Tasks.Status IS NOT NULL AND Tasks.Status <> 0)
        AND Tasks.Hidden <> 1;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetUnstartedDueForStartForAssignedUser`;

CREATE PROCEDURE `testPTSDB`.`GetUnstartedDueForStartForAssignedUser`
(
    IN `assignedID` INT UNSIGNED,
    IN `planStart` DATE
)

BEGIN

    SELECT * FROM Tasks
    WHERE Tasks.AsignedTo = assignedID
        AND Tasks.ScheduledStart < planStart
        AND (Tasks.Status IS NULL OR Tasks.Status = 0)
        AND Tasks.Hidden <> 1;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetTasksCompletedByAssignedAfterDate`;

CREATE PROCEDURE `testPTSDB`.`GetTasksCompletedByAssignedAfterDate`
(
    IN `assignedID` INT UNSIGNED,
    IN `searchStartDate` DATE
)

BEGIN

    SELECT * FROM Tasks
    WHERE Tasks.AsignedTo = assignedID
        AND Tasks.Completed >= searchStartDate;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetTasksByAssignedIDandParentID`;

CREATE PROCEDURE `testPTSDB`.`GetTasksByAssignedIDandParentID`
(
    IN `assignedID` INT UNSIGNED,
    IN `parentID` INT UNSIGNED
)

BEGIN

    SELECT * FROM Tasks
    WHERE Tasks.AsignedTo = assignedID
        AND Tasks.ParentTask = parentID
        AND Tasks.Hidden <> 1;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetDefaultDashboardTaskList`;

CREATE PROCEDURE `testPTSDB`.`GetDefaultDashboardTaskList`
(
    IN `assignedID` INT UNSIGNED,
    IN `dueDate` DATE
)

BEGIN

    SELECT * FROM Tasks
    WHERE Tasks.AsignedTo = assignedID
        AND Tasks.Completed IS NULL
        AND Tasks.Hidden <> 1
        AND (Tasks.Status = 3 OR Tasks.RequiredDelivery < dueDate)
    ORDER BY Tasks.SchedulePriorityGroup ASC, Tasks.PriorityInGroup ASC;

END$$

DELIMITER ;
-- --------------------------------------------------------
-- End of Task related stored procedures
-- --------------------------------------------------------

-- --------------------------------------------------------

DROP TABLE IF EXISTS `testPTSDB`.`UserTaskGoals`;
CREATE TABLE IF NOT EXISTS  `testPTSDB`.`UserTaskGoals` (
    `UserID` INT UNSIGNED NOT NULL,
    `TaskID`  INT UNSIGNED NOT NULL,
    `TaskGoalList` VARCHAR(45) NOT NULL,
    `CreationTS` DATETIME NOT NULL,
    `LastUpdateTS` DATETIME NOT NULL,
    `Hidden` TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
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
-- Stored Procedures for UserTaskGoals
-- --------------------------------------------------------

USE `testPTSDB`;

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
    `Hidden` TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
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

-- --------------------------------------------------------
-- Stored Procedures for UserScheduleItem
-- --------------------------------------------------------

USE `testPTSDB`;

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

DROP PROCEDURE IF EXISTS `testPTSDB`.`HideScheduleItem`;

CREATE PROCEDURE `testPTSDB`.`HideScheduleItem`
(
    IN IDUser INT,
    IN ScheduleItemID INT
)

BEGIN

    UPDATE UserScheduleItem
        SET Hidden = 1, LastUpdateTS = UTC_TIMESTAMP()
        WHERE UserID = IDUser AND idUserScheduleItem = ScheduleItemID;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetScheduleItemById`;

CREATE PROCEDURE `testPTSDB`.`GetScheduleItemById`
(
    IN scheduleItemID INT UNSIGNED 
)

BEGIN

    SELECT * FROM UserScheduleItem
    WHERE UserScheduleItem.idUserScheduleItem = scheduleItemID
        AND UserScheduleItem.Hidden <> 1;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`GetUserDaySchedule`;

CREATE PROCEDURE `testPTSDB`.`GetUserDaySchedule`
(
    IN `userId` INT UNSIGNED,
    IN `eventStart` DATETIME,
    IN `eventEnd` DATETIME
)

BEGIN

    SELECT * FROM UserScheduleItem
    WHERE UserScheduleItem.UserID = userId
        AND UserScheduleItem.Hidden <> 1
        AND UserScheduleItem.StartDateTime >= eventStart
        AND UserScheduleItem.StartDateTime <= eventEnd
    ORDER BY UserScheduleItem.StartDateTime ASC;

END$$

DROP PROCEDURE IF EXISTS `testPTSDB`.`FindUserScheduleItemsByContentAndDateRange`;

CREATE PROCEDURE `testPTSDB`.`FindUserScheduleItemsByContentAndDateRange`
(
    IN `userId` INT UNSIGNED,
    IN `matchContent`VARCHAR(128),
    IN `searchStart` DATE,
    IN `searchEnd` DATE
)

BEGIN

    SELECT * FROM UserScheduleItem  WHERE UserID = userId
    AND Title LIKE CONCAT('%', matchContent, '%')
    AND Hidden <> 1
    AND StartDateTime >= searchStart
    AND StartDateTime <= searchEnd;

END$$

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
        UTC_TIMESTAMP(),
        UTC_TIMESTAMP(),
        0
    )
    RETURNING idUserScheduleItem;
    
END$$

CREATE PROCEDURE `UpdateScheduleItemAllFields`
(
    IN `userId` INT UNSIGNED,
    IN `eventId` INT UNSIGNED,
    IN `startTime` DATETIME,
    IN `endTime` DATETIME,
    IN `title` VARCHAR(128),
    IN `personal` TINYINT,
    IN `location` VARCHAR(128)
)
BEGIN

    UPDATE UserScheduleItem SET
        UserScheduleItem.StartDateTime = startTime,
        UserScheduleItem.EndDateTime = endTime,
        UserScheduleItem.Title = title,
        UserScheduleItem.Personal = personal,
        UserScheduleItem.Location = location,
        UserScheduleItem.LastUpdateTS = UTC_TIMESTAMP()
    WHERE UserScheduleItem.idUserScheduleItem = eventId AND UserScheduleItem.UserID = userId;
    
END$$
DELIMITER ;

COMMIT;


