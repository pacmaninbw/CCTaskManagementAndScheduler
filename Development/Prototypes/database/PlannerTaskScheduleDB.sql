SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;

CREATE DATABASE IF NOT EXISTS `PlannerTaskScheduleDB`;

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserProfile`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserProfile` (
    `UserID` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `LastName` VARCHAR(45) NOT NULL,
    `FirstName` TINYTEXT NOT NULL,
    `MiddleInitial` TINYTEXT,
    PRIMARY KEY (`UserID`, `LastName`),
    UNIQUE INDEX `UserID_UNIQUE` (`UserID`)
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserLoginAndPassword`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserLoginAndPassword` (
    `UserID` INT UNSIGNED NOT NULL,
    `LoginName` VARCHAR(45) NOT NULL,
    `HashedPassWord` TINYTEXT,
    PRIMARY KEY (`UserID`, `LoginName`),
    UNIQUE INDEX `UserID_UNIQUE` (`UserID` ASC),
    UNIQUE INDEX `LoginName_UNIQUE` (`LoginName` ASC),
    CONSTRAINT `fk_PassWord_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`UserID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS  `PlannerTaskScheduleDB`.`UserPlatformIndependentPreferences`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserPlatformIndependentPreferences` (
    `UserID` INT UNSIGNED NOT NULL,
    `ScheduleDayStart` TIME NOT NULL,
    `ScheduleDayEnd` TIME NOT NULL,
    `IncludePriorityInSchedule` BOOLEAN DEFAULT TRUE,
    `IncludeMinorPriorityInSchedule` BOOLEAN DEFAULT TRUE,
    `UseLettersForMajorPriority` BOOLEAN DEFAULT TRUE,
    `SeparatePriorityWithDot` BOOLEAN DEFAULT FALSE,
    PRIMARY KEY (`UserID`),
    UNIQUE INDEX `UserID_UNIQUE` (`UserID` ASC),
    CONSTRAINT `fk_PlatformIndependentPrefs_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`UserID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
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
    UNIQUE INDEX `idUserNotes_UNIQUE` (`idUserNotes` ASC) VISIBLE,
    INDEX `fk_UserNotes_UserID_idx` (`UserID` ASC) VISIBLE,
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
    `PriorityInAllTasks` INT UNSIGNED NOT NULL,
    `ParentTask` INT UNSIGNED DEFAULT NULL,
    `Status` INT UNSIGNED DEFAULT NULL,
    `PercentageComplete` double NOT NULL,
    PRIMARY KEY (`TaskID`, `CreatedBy`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID` ASC) VISIBLE,
    INDEX `fk_Tasks_CreatedBy_idx` (`CreatedBy` ASC) VISIBLE,
    INDEX `fk_Tasks_AsignedTo_idx` (`AsignedTo` ASC) VISIBLE,
    INDEX `Description_idx` (`Description` ASC) VISIBLE,
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

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`TaskDates`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`TaskDates` (
    `TaskID` INT UNSIGNED NOT NULL,
    `CreatedOn` date NOT NULL,
    `RequiredDelivery` date NOT NULL,
    `ScheduledStart` date NOT NULL,
    `ActualStart` date DEFAULT NULL,
    `EstimatedCompletion` date DEFAULT NULL,
    `Comleted` date DEFAULT NULL,
    PRIMARY KEY (`TaskID`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID` ASC),
    CONSTRAINT `fk_TaskDates_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`TaskID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`TaskEffort`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`TaskEffort` (
    `TaskID` INT UNSIGNED NOT NULL,
    `EstimatedEffortHours` INT UNSIGNED NOT NULL,
    `ActualEffortHours` float NOT NULL,
    PRIMARY KEY (`TaskID`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID`),
    CONSTRAINT `fk_TaskEffort_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`TaskID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `PlannerTaskScheduleDB`.`UserTaskPriority`;
CREATE TABLE IF NOT EXISTS  `PlannerTaskScheduleDB`.`UserTaskPriority` (
    `TaskID` INT UNSIGNED NOT NULL,
    `UserID` INT UNSIGNED NOT NULL,
    `SchedulePriorityGroup` INT UNSIGNED NOT NULL,
    `PriorityInGroup` INT UNSIGNED NOT NULL,
    PRIMARY KEY (`TaskID`, `UserID`),
    UNIQUE INDEX `TaskID_UNIQUE` (`TaskID` ASC),
    CONSTRAINT `fk_Priority_TaskID`
        FOREIGN KEY (`TaskID`)
        REFERENCES `Tasks` (`TaskID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT `fk_Priority_UserID`
        FOREIGN KEY (`UserID`)
        REFERENCES `UserProfile` (`UserID`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

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
    UNIQUE INDEX `idUserDaySchedule_UNIQUE` (`idUserDaySchedule` ASC) VISIBLE,
    INDEX `fk_UserDaySchedule_UserID_idx` (`UserID` ASC) VISIBLE,
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
    UNIQUE INDEX `idUserScheduleItem_UNIQUE` (`idUserScheduleItem` ASC) VISIBLE,
    INDEX `fk_UserScheduleItem_UserID_idx` (`UserID` ASC) VISIBLE,
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

DELIMITER;

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

	INSERT INTO UserPlatformIndependentPreferences (
		UserPlatformIndependentPreferences.UserID,
        UserPlatformIndependentPreferences.ScheduleDayStart,
        UserPlatformIndependentPreferences.ScheduleDayEnd
    )
    
    VALUES (UserID, '08:30:00', '17:00:00');
    
END$$

DELIMITER ;


USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `addNewUserUserLoginAndPassword`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `addNewUserUserLoginAndPassword`
(
	IN UserID INT UNSIGNED,
    IN LoginName VARCHAR(45),
    IN HashedPassWord TINYTEXT
)
BEGIN

	INSERT INTO UserLoginAndPassword
		(
			UserLoginAndPassword.UserID,
            UserLoginAndPassword.LoginName,
            UserLoginAndPassword.HashedPassWord
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
        
        CALL addNewUserUserLoginAndPassword(@NewUserID, LoginName, HashedPassWord);
        
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
    IN Description VARCHAR(256),
    IN ParentTask INT UNSIGNED,
    IN EstimatedEffortHours INT UNSIGNED,
    IN PriorityInAllTasks INT UNSIGNED,
    IN RequiredDelivery DATE,
    IN ScheduledStart DATE
)
BEGIN

	SET @TaskStatusEnumValue = findTaskStatusEnumValueByLabel('Not Started');
    
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
            @TaskStatusEnumValue
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
            WHERE Tasks.TaskID = @NewTaskID;
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
    
    INSERT INTO UserNotes
		(
			UserNotes.UserID,
            UserNotes.NotationDateTime,
            UserNotes.Content
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

	INSERT INTO UserGoals
		(
			UserGoals.UserID,
            UserGoals.Description
        )
        VALUES
        (
			UserID,
            Description
        );
        
	SET @idUserGoals  := LAST_INSERT_ID();
    
	IF Priority IS NOT NULL AND Priority > 0 THEN
        UPDATE UserGoals 
            SET UserGoals.Priority = Priority
            WHERE UserGoals.TaskID = @idUserGoals;
    END IF;

	IF ParentGoal IS NOT NULL AND ParentGoal > 0 THEN
        UPDATE UserGoals 
            SET UserGoals.ParentGoal = ParentGoal
            WHERE UserGoals.TaskID = @idUserGoals;
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

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `addUserScheduleItem`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `addUserScheduleItem`
(
	IN UserID INT UNSIGNED,
    IN StartTime DATETIME,
    IN EndTime DATETIME,
    IN ItemType VARCHAR(45),
    IN Title VARCHAR(45),
    IN Location VARCHAR(45)
)
BEGIN
	SET @ScheduleItemTypeID =
		findUserScheduleItemTypeEnumValueByLabel(ItemType);
    
	INSERT INTO UserScheduleItem
		(
			UserScheduleItem.UserID,
            UserScheduleItem.StartDateTime,
            UserScheduleItem.EndDateTime,
            UserScheduleItem.ItemType,
            UserScheduleItem.Title,
            UserScheduleItem.Location
		)
		VALUES
		(
			UserID,
            StartTime,
            EndTime,
            @ScheduleItemTypeID,
            Title,
            Location
		);
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `addUserDaySchedule`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `addUserDaySchedule`
(
	IN UserID INT UNSIGNED,
    IN DateOfSchedule DATE,
    IN StartOfDay TIME,
    IN EndOfDay TIME,
    IN DailyGoals VARCHAR(45)
)
BEGIN
	
	IF DateOfSchedule IS NULL THEN
		SET DateOfSchedule = current_date();
    END IF;
    
    INSERT INTO UserDaySchedule
		(
			UserDaySchedule.UserID,
            UserDaySchedule.DateOfSchedule,
            UserDaySchedule.StartOfDay,
            UserDaySchedule.EndOfDay,
            UserDaySchedule.DailyGoals
        )
        VALUES
        (
			UserID,
            DateOfSchedule,
            StartOfDay,
            EndOfDay,
            DailyGoals
        );
    
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `todaysTaskList`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `todaysTaskList`
(
	IN UserID INT UNSIGNED
)
BEGIN

	SET @TaskListDate = current_date();
    
	SELECT
		UTP.SchedulePriorityGroup,
		UTP.PriorityInGroup,
        t.Description
	FROM 
        Tasks as t
        LEFT JOIN UserTaskPriority as UTP ON t.TaskID = UTP.TaskID
        LEFT JOIN TaskDates as TD ON t.TaskID = TD.TaskID
	WHERE
		t.AsignedTo = UserID AND
        UTP.UserID = UserID AND
        TD.Comleted IS NULL AND
        TD.RequiredDelivery <= @TaskListDate
    ORDER BY 
        UTP.SchedulePriorityGroup ASC,
        UTP.PriorityInGroup ASC,
        TD.RequiredDelivery ASC
    ;
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `addUserTaskByLoginName`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `addUserTaskByLoginName`
(
	IN LoginName VARCHAR(45),
	IN Description VARCHAR(256),
    IN ParentTask INT UNSIGNED,
    IN EstimatedEffortHours INT UNSIGNED,
    IN PriorityInAllTasks INT UNSIGNED,
    IN RequiredDelivery DATE,
    IN ScheduledStart DATE
)
BEGIN

	SET @CreatedBy = findUserIDKeyByLoginName(LoginName);
    
    CALL createTask(
		@CreatedBy,
        Description,
        ParentTask,
        EstimatedEffortHours,
        PriorityInAllTasks,
        RequiredDelivery,
        ScheduledStart
	);
END$$

DELIMITER ;

COMMIT;

