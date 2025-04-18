USE `PlannerTaskScheduleDB`;

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


