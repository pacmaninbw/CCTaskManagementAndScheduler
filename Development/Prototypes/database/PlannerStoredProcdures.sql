USE `PlannerTaskScheduleDB`;

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
    
    INSERT INTO Dates
		(
			Dates.TaskID,
            Dates.CreatedOn,
            Dates.RequiredDelivery,
            Dates.ScheduledStart
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

	INSERT INTO Effort
		(
			Effort.TaskID,
            Effort.EstimatedEffortHours,
            Effort.ActualEffortHours
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

	INSERT INTO UserPriority
		(
			UserPriority.TaskID,
            UserPriority.UserID,
            UserPriority.Level,
            UserPriority.PriorityInLevel
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
        FROM Effort
        WHERE Effort.TaskID = TaskID;
        
	SET @AccumlatedEffort = @AccumlatedEffort + ActualEffortHours;
    
    UPDATE Effort 
		SET Effort.ActualEffortHours = @AccumlatedEffort
        WHERE Effort.TaskID = TaskID;
        
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

	INSERT INTO Dependencies
		(
			Dependencies.TaskID,
            Dependencies.Dependency
        )
        VALUES
        (
			TaskID,
            Dependency
        );
END$$

DELIMITER ;

