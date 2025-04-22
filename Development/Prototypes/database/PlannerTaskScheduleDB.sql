SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;

CREATE DATABASE IF NOT EXISTS `PlannerTaskScheduleDB`;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserProfile`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserProfile` (
    `idUserProfile` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `LastName` VARCHAR(45) NOT NULL,
    `FirstName` TINYTEXT NOT NULL,
    `MiddleInitial` TINYTEXT,
    PRIMARY KEY (`idUserProfile`, `LastName`),
    UNIQUE INDEX `idUserProfile_UNIQUE` (`idUserProfile`)
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`Tasks`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`Tasks` (
    `idTasks` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `CreatedBy` INT UNSIGNED NOT NULL,
    `AsignedTo` INT UNSIGNED NOT NULL,
    `Description` TINYTEXT NOT NULL,
    `PriorityInAllTasks` INT UNSIGNED NOT NULL,
    `ParentTask` INT UNSIGNED DEFAULT NULL,
    `Status` INT UNSIGNED DEFAULT NULL,
    `PercentageComplete` double NOT NULL,
    PRIMARY KEY (`idTasks`, `CreatedBy`),
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
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`TaskDates`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`TaskDates` (
    `idTaskDates` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` INT UNSIGNED NOT NULL,
    `CreatedOn` date NOT NULL,
    `RequiredDelivery` date NOT NULL,
    `ScheduledStart` date NOT NULL,
    `ActualStart` date DEFAULT NULL,
    `EstimatedCompletion` date DEFAULT NULL,
    `Comleted` date DEFAULT NULL,
    PRIMARY KEY (`idTaskDates`, `TaskID`),
    UNIQUE INDEX `idTaskDates_UNIQUE` (`idTaskDates` ASC),
    INDEX `fk_TaskDates_TaskID_idx` (`TaskID` ASC),
    CONSTRAINT `fk_TaskDates_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`TaskEffort`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`TaskEffort` (
    `idTaskEffort` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` INT UNSIGNED NOT NULL,
    `EstimatedEffortHours` INT UNSIGNED NOT NULL,
    `ActualEffortHours` float NOT NULL,
    PRIMARY KEY (`idTaskEffort`, `TaskID`),
    UNIQUE INDEX `idTaskEffort_UNIQUE` (`idTaskEffort`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID`),
    CONSTRAINT `fk_TaskEffort_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`Goals`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`Goals` (
    `idGoals` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `Description` TINYTEXT NOT NULL,
    `Priority` INT DEFAULT NULL,
    `ParentGoal` INT UNSIGNED DEFAULT NULL,
    PRIMARY KEY (`idGoals`, `UserID`),
    UNIQUE INDEX `idGoals_UNIQUE` (`idGoals`),
    CONSTRAINT `fk_Goals_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX `fk_Goals_UserID_idx` (`UserID`)
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`LoginAndPassword`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`LoginAndPassword` (
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
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserTaskPriority`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserTaskPriority` (
    `idUserTaskPriority` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `TaskID` INT UNSIGNED NOT NULL,
    `UserID` INT UNSIGNED NOT NULL,
    `SchedulePriorityGroup` INT UNSIGNED NOT NULL,
    `PriorityInGroup` INT UNSIGNED NOT NULL,
    PRIMARY KEY (`idUserTaskPriority`, `TaskID`, `UserID`),
    UNIQUE INDEX `idUserTaskPriority_UNIQUE` (`idUserTaskPriority` ASC),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID` ASC),
    CONSTRAINT `fk_Priority_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT `fk_Priority_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserTaskGoals`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserTaskGoals` (
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
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`TaskStatus`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`TaskStatus` (
    `idTaskStatus` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `StatusStr` VARCHAR(45) NOT NULL,
    PRIMARY KEY (`idTaskStatus`),
    UNIQUE INDEX `idTaskStatus_UNIQUE` (`idTaskStatus` ASC),
    UNIQUE INDEX `StatusStr_UNIQUE` (`StatusStr` ASC)
);

INSERT INTO PlannerTaskScheduleDB.TaskStatus
    (StatusStr)
    VALUES
        ('Not Started'),
        ('On Hold'),
        ('Waiting for Dependency'),
        ('Work in Progress'),
        ('Complete');

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
        REFERENCES `Tasks` (`idTasks`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS  `PlannerTaskScheduleDB`.`PlatformIndependentPreferences`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`PlatformIndependentPreferences` (
    `idPlatformIndependentPreferences` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `ScheduleDayStart` TIME NOT NULL,
    `ScheduleDayEnd` TIME NOT NULL,
    `IncludePriorityInSchedule` BOOLEAN DEFAULT TRUE,
    `IncludeMinorPriorityInSchedule` BOOLEAN DEFAULT TRUE,
    `UseLettersForMajorPriority` BOOLEAN DEFAULT TRUE,
    `SeparatePriorityWithDot` BOOLEAN DEFAULT FALSE,
    PRIMARY KEY (`idPlatformIndependentPreferences`, `UserID`),
    UNIQUE INDEX `idPlatformIndependentPreferences_UNIQUE` (`idPlatformIndependentPreferences` ASC),
    UNIQUE INDEX `UserID_UNIQUE` (`UserID` ASC),
    CONSTRAINT `fk_PlatformIndependentPrefs_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`idUserProfile`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS  `PlannerTaskScheduleDB`.`ScheduleItemType`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`ScheduleItemType` (
    `idScheduleItemType` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `ScheduleItemTypeStr` VARCHAR(45) NOT NULL,
    PRIMARY KEY (`idScheduleItemType`),
    UNIQUE INDEX `idScheduleItemType_UNIQUE` (`idScheduleItemType` ASC),
    UNIQUE INDEX `ScheduleItemTypeStr_UNIQUE` (`ScheduleItemTypeStr` ASC)
);

INSERT INTO PlannerTaskScheduleDB.ScheduleItemType
    (ScheduleItemTypeStr)
    VALUES
        ('Meeting'),
        ('Phone Call'),
        ('Task Execution'),
        ('Personal Appointment'),
        ('Personal Other');


-- --------------------------------------------------------

DROP TABLE IF EXISTS  `PlannerTaskScheduleDB`.`DaySchedule`;
CREATE TABLE IF NOT EXISTS `PlannerTaskScheduleDB`.`DaySchedule` (
    `idDaySchedule` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `DateOfSchedule` DATE NOT NULL,
    `StartOfDay` TIME NOT NULL,
    `EndOfDay` TIME NOT NULL,
    `DailyGoals` VARCHAR(45) NULL,
    PRIMARY KEY (`idDaySchedule`, `UserID`),
    UNIQUE INDEX `idDaySchedule_UNIQUE` (`idDaySchedule` ASC) VISIBLE,
    INDEX `fk_DaySchedule_UserID_idx` (`UserID` ASC) VISIBLE,
    CONSTRAINT `fk_DaySchedule_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `PlannerTaskScheduleDB`.`UserProfile` (`idUserProfile`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS  `PlannerTaskScheduleDB`.`ScheduleItem`;
CREATE TABLE IF NOT EXISTS `PlannerTaskScheduleDB`.`ScheduleItem` (
    `idScheduleItem` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `StartDateTime` DATETIME NOT NULL,
    `EndDateTime` DATETIME NOT NULL,
    `ItemType` TINYINT NOT NULL,
    `Location` VARCHAR(45) DEFAULT NULL,
    `TaskID` INT UNSIGNED DEFAULT NULL,
    PRIMARY KEY (`idScheduleItem`, `UserID`),
    UNIQUE INDEX `idScheduleItem_UNIQUE` (`idScheduleItem` ASC) VISIBLE,
    INDEX `fk_ScheduleItem_UserID_idx` (`UserID` ASC) VISIBLE,
    CONSTRAINT `fk_ScheduleItem_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `PlannerTaskScheduleDB`.`UserProfile` (`idUserProfile`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`Notes`;
CREATE TABLE IF NOT EXISTS `PlannerTaskScheduleDB`.`Notes` (
    `idNotes` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `UserID` INT UNSIGNED NOT NULL,
    `NotationDateTime` DATETIME NOT NULL,
    `Content` VARCHAR(1024) NOT NULL,
    PRIMARY KEY (`idNotes`, `UserID`),
    UNIQUE INDEX `idNotes_UNIQUE` (`idNotes` ASC) VISIBLE,
    INDEX `fk_Notes_UserID_idx` (`UserID` ASC) VISIBLE,
    CONSTRAINT `fk_Notes_UserID`
      FOREIGN KEY (`UserID`)
      REFERENCES `PlannerTaskScheduleDB`.`UserProfile` (`idUserProfile`)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);
    
-- -----------------------------------------------------
-- Stored Functions
-- -----------------------------------------------------

-- -----------------------------------------------------
-- function findTaskStatusValueByLabel
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findTaskStatusValueByLabel`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findTaskStatusValueByLabel`(
    StatusStr VARCHAR(20)
) RETURNS INT
DETERMINISTIC
BEGIN
    
    SET @TaskStatusKey = 0;
    
    SELECT TaskStatus.idTaskStatus INTO @TaskStatusKey
        FROM TaskStatus
        WHERE TaskStatus.StatusStr = StatusStr;
    IF @TaskStatusKey IS NULL THEN
        SET @TaskStatusKey = 0;
    END IF;

    RETURN @TaskStatusKey;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function findTaskStatusLabelByValue
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findTaskStatusLabelByValue`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findTaskStatusLabelByValue`(
    StatusKey INT
) RETURNS VARCHAR(20)
DETERMINISTIC
BEGIN
    
    SET @TaskStatusLabel = ``;
    
    SELECT TaskStatus.StatusStr INTO @TaskStatusLabel
        FROM TaskStatus
        WHERE TaskStatus.idTaskStatus = StatusStr;
    IF @TasTaskStatusLabelkStatusKey IS NULL THEN
        SET @TaskStatusLabel = 'Not Started';
    END IF;

    RETURN @TaskStatusLabel;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function findScheduleItemTypeLabelByValue
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findScheduleItemTypeLabelByValue`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findScheduleItemTypeLabelByValue`(
    StatusKey INT
) RETURNS VARCHAR(20)
DETERMINISTIC
BEGIN
    
    SET @ScheduleItemTypeLabel = ``;
    
    SELECT ScheduleItemType.StatusStr INTO @ScheduleItemTypeLabel
        FROM ScheduleItemType
        WHERE ScheduleItemType.idScheduleItemType = StatusStr;
    IF @TasScheduleItemTypeLabelkStatusKey IS NULL THEN
        SET @ScheduleItemTypeLabel = 'Not Started';
    END IF;

    RETURN @ScheduleItemTypeLabel;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- function findScheduleItemTypeValueByLabel
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `PlannerTaskScheduleDB`.`findScheduleItemTypeValueByLabel`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `findScheduleItemTypeValueByLabel`(
    StatusStr VARCHAR(20)
) RETURNS INT
DETERMINISTIC
BEGIN
    
    SET @ScheduleItemTypeKey = 0;
    
    SELECT ScheduleItemType.idScheduleItemType INTO @ScheduleItemTypeKey
        FROM ScheduleItemType
        WHERE ScheduleItemType.StatusStr = StatusStr;
    IF @ScheduleItemTypeKey IS NULL THEN
        SET @ScheduleItemTypeKey = 0;
    END IF;

    RETURN @ScheduleItemTypeKey;
    
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

    SELECT LoginAndPassword.UserID INTO @UserIDKey
        FROM LoginAndPassword
        WHERE LoginAndPassword.LoginName = LoginName;
    IF @UserIDKey IS NULL THEN
        SET @UserIDKey = 0;
    END IF;

    RETURN @UserIDKey;
    
END$$

DELIMITER;

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

DELIMITER;

-- -----------------------------------------------------
-- function isValidLoginAndPassword
-- -----------------------------------------------------

USE `PlannerTaskScheduleDB`;
DROP function IF EXISTS `isValidLoginAndPassword`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE FUNCTION `isValidLoginAndPassword`
(
    LoginName VARCHAR(45),
    HashedPassWord TINYTEXT
)
RETURNS TINYINT
DETERMINISTIC
BEGIN

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

-- -----------------------------------------------------
-- Stored Procedures
-- -----------------------------------------------------


USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `addNewUserPreferences`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `addNewUserPreferences`
(
	IN UserID INT UNSIGNED
)
BEGIN

	INSERT INTO PlatformIndepenentPreferences (
		PlatformIndepenentPreferences.UserID,
        PlatformIndepenentPreferences.ScheduleDayStart,
        PlatformIndepenentPreferences.ScheduleDayEnd
    )
    
    VALUES (UserID, '8:30:00', '17:00:00');
    
END$$

DELIMITER ;


USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `addNewUserLoginAndPassword`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `addNewUserLoginAndPassword`
(
	IN UserID INT UNSIGNED,
    IN LoginName VARCHAR(45),
    IN HashedPassWord TINYTEXT
)
BEGIN

	INSERT INTO LoginAndPassword
		(
			LoginAndPassword.UserID,
            LoginAndPassword.LoginName,
            LoginAndPassword.HashedPassWord
		)
		VALUES (UserID, LoginName, HashedPassWord);
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `addNewUser`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `addNewUser`
	(
        IN LastName VARCHAR(45),
		IN FirstName TINYTEXT,
		IN MiddleInitial TINYTEXT,
	    IN LoginName VARCHAR(45),
		IN HashedPassWord TINYTEXT
    )
BEGIN

	INSERT INTO UserProfile
		(
			UserProfile.LastName,
            UserProfile.FirstName,
            UserProfile.MiddleInitial
        )
		VALUES (LastName, FirstName, MiddleInitial);
        
        SET @NewUserID := LAST_INSERT_ID();
        
        CALL addNewUserLoginAndPassword(@NewUserID, LoginName, HashedPassWord);
        
        CALL addNewUserPreferences(@NewUserID);

END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `createTaskDates`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `createTaskDates`
(
	TaskID INT UNSIGNED,
    RequiredDelivery DATE,
    ScheduledStart DATE
)
BEGIN
	SET @CreationDate := curdate();
    
    INSERT INTO TaskDates
		(
			TaskDates.TaskID,
            TaskDates.CreatedOn,
            TaskDates.RequiredDelivery,
            TaskDates.ScheduledStart
		)
		VALUES (
			TaskID,
            @CreationDate,
            RequiredDelivery,
            ScheduledStart
		);
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `createTaskEffort`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `createTaskEffort`
(
	IN TaskID INT UNSIGNED,
    IN EstimatedEffortHours INT UNSIGNED
)
BEGIN

	INSERT INTO TaskEffort
		(
			TaskEffort.TaskID,
            TaskEffort.EstimatedEffortHours,
            TaskEffort.ActualEffortHours
		)
        VALUES
        (
			TaskID,
            EstimatedEffortHours,
            0.0
        );
    
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `createTask`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `createTask`
(
	IN CreatedBy INT UNSIGNED,
    IN Description TINYTEXT,
    IN ParentTask INT UNSIGNED,
    IN EstimatedEffortHours INT UNSIGNED,
    IN PriorityInAllTasks INT UNSIGNED,
    IN RequiredDelivery DATE,
    IN ScheduledStart DATE
)
BEGIN

	SET @TaskStatusValue = findTaskStatusValueByLabel('Not Started');
    
	INSERT INTO Tasks
		(
			Tasks.CreatedBy,
            Tasks.AsignedTo,
            Tasks.Description,
            Tasks.PriorityInAllTasks,
            Tasks.Status
        )
		VALUES
		(
			CreatedBy,
            CreatedBy,
            Description,
            PriorityInAllTasks,
            @TaskStatusValue
		);
        
		SET @NewTaskID := LAST_INSERT_ID();
        
        CALL createTaskDates(
			@NewTaskID,
            RequiredDelivery,
            ScheduledStart
		);
        
        CALL createTaskEffort
        (
			@NewTaskID,
            EstimatedEffortHours
        );

    IF ParentTask IS NOT NULL AND ParentTask > 0 THEN
        UPDATE Tasks 
            SET Tasks.ParentTask = ParentTask
            WHERE Tasks.idTasks = @NewTaskID;
    END IF;

END$$

DELIMITER ;


USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `createUserTaskPriority`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `createUserTaskPriority`
(
	IN UserID INT UNSIGNED,
    IN TaskID INT UNSIGNED,
    IN SchedulePriorityGroup INT UNSIGNED,
    IN PriorityInGroup INT UNSIGNED
)
BEGIN

	INSERT INTO UserTaskPriority
		(
			UserTaskPriority.TaskID,
            UserTaskPriority.UserID,
            UserTaskPriority.SchedulePriorityGroup,
            UserTaskPriority.PriorityInGroup
		)
        VALUES
        (
			TaskID,
            UserID,
            SchedulePriorityGroup,
            PriorityInGroup
        );
END$$

DELIMITER ;


USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `creatNote`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `creatNote`
(
	IN UserID INT UNSIGNED,
    IN Content VARCHAR(1024)
)
BEGIN

	SET @NotationDateTime := now();
    
    INSERT INTO Notes
		(
			Notes.UserID,
            Notes.NotationDateTime,
            Notes.Content
		)
		VALUES
		(
			UserID,
            @NotationDateTime,
            Content
        );

END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `createGoal`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `createGoal`
(
	IN UserID INT UNSIGNED,
    IN Description TINYTEXT,
    IN Priority INT,
    IN ParentGoal INT UNSIGNED
)
BEGIN

	INSERT INTO Goals
		(
			Goals.UserID,
            Goals.Description
        )
        VALUES
        (
			UserID,
            Description
        );
        
	SET @idGoals  := LAST_INSERT_ID();
    
	IF Priority IS NOT NULL AND Priority > 0 THEN
        UPDATE Goals 
            SET Goals.Priority = Priority
            WHERE Goals.idTasks = @idGoals;
    END IF;

	IF ParentGoal IS NOT NULL AND ParentGoal > 0 THEN
        UPDATE Goals 
            SET Goals.ParentGoal = ParentGoal
            WHERE Goals.idTasks = @idGoals;
    END IF;

END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `accumulateTaskEffort`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `accumulateTaskEffort`
(
	IN TaskID INT UNSIGNED,
    IN ActualEffortHours FLOAT
)
BEGIN

	SELECT ActualEffortHours
		INTO @AccumlatedEffort
        FROM TaskEffort
        WHERE TaskEffort.TaskID = TaskID;
        
	SET @AccumlatedEffort = @AccumlatedEffort + ActualEffortHours;
    
    UPDATE Effort 
		SET TaskEffort.ActualEffortHours = @AccumlatedEffort
        WHERE TaskEffort.TaskID = TaskID;
        
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `addTaskDependency`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `addTaskDependency`
(
	IN TaskID INT UNSIGNED,
    IN Dependency INT UNSIGNED
)
BEGIN

	INSERT INTO TaskDependencies
		(
			TaskDependencies.TaskID,
            TaskDependencies.Dependency
        )
        VALUES
        (
			TaskID,
            Dependency
        );
END$$

DELIMITER ;



COMMIT;
